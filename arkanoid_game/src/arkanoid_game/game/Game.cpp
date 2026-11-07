#include "./Game.h"

#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

#include "arkanoid_lib/level/LevelData.h"
#include "arkanoid_lib/resources/ResourceSystem.h"
#include "arkanoid_lib/level/BrickTypes.h"

#include "arkanoid_game/game/GameSettings.h"

namespace arkanoid::game
{
	Game::Game(SDL_Window& a_Window, SDL_Renderer& a_Renderer) :
		m_Window(a_Window),
		m_Renderer(a_Renderer),
		m_ResourceSystem(a_Renderer),
		m_TextSystem(a_Renderer),
		m_LevelManager(m_ResourceSystem),
		m_GameAudioManager(m_AudioSystem)
	{
		SDL_Log("Working directory: %s", SDL_GetCurrentDirectory());
	}

	void Game::Initialize()
	{
		m_World.Initialize();
		m_GameAudioManager.Initialize();
	}

	void Game::Start()
	{
		m_LevelManager.LoadLevel(0, m_World.GetLevel());
		ResetLives();
	}

	void Game::LoadTextures()
	{
		m_ResourceSystem.LoadTexture(resources::ETextureType::DUMMY, "tex_dummy.png"); // Used for measuring the bricks.

		SDL_Texture* m_pDummyTexture = m_ResourceSystem.GetTexture(resources::ETextureType::DUMMY);
		if (!m_pDummyTexture || !SDL_GetTextureSize(m_pDummyTexture, &m_fDummyWidth, &m_fDummyHeight))
		{
			m_fDummyWidth = level::DUMMY_DEFAULT_WIDTH;
			m_fDummyHeight = level::DUMMY_DEFAULT_HEIGHT;
		}

		// Not my best work ever, but it ensures that a row will always fit the screen.
		m_fGameWidth = m_fDummyWidth * level::LEVEL_WIDTH;
		m_fGameHeight = m_fDummyHeight * (level::PADDLE_POS_Y + 5);

		m_ResourceSystem.LoadTexture(resources::ETextureType::BG, "tex_bg.png"); // Used for measuring the bricks.

		m_ResourceSystem.LoadTexture(resources::ETextureType::BRICK, "tex_bricks.png");
		m_ResourceSystem.LoadTexture(resources::ETextureType::BRICK_SHINE_OVERLAY, "tex_brick_shine.png");
		m_ResourceSystem.LoadTexture(resources::ETextureType::PADDLE, "tex_paddle.png");
		m_ResourceSystem.LoadTexture(resources::ETextureType::BALL, "tex_ball.png");
		m_ResourceSystem.LoadTexture(resources::ETextureType::EXPLOSION, "tex_explosion.png");
		m_ResourceSystem.LoadTexture(resources::ETextureType::POWER_UP, "tex_powerups.png");
		m_ResourceSystem.LoadTexture(resources::ETextureType::LASER, "tex_laser.png");

		m_World.LoadTextures(m_ResourceSystem);
	}

	void Game::LoadSounds()
	{
		m_GameAudioManager.LoadSounds(m_ResourceSystem);
	}

	void Game::Destroy()
	{
		m_ResourceSystem.Destroy();
		m_AudioSystem.Destroy();
		m_TextSystem.Destroy();
	}

	float Game::GetDummyWidth() const
	{
		return m_fDummyWidth;
	}

	float Game::GetDummyHeight() const
	{
		return m_fDummyHeight;
	}

	float Game::GetGameWidth() const
	{
		return m_fGameWidth;
	}

	float Game::GetGameHeight() const
	{
		return m_fGameHeight;
	}

	void Game::Update(float a_fDeltaTime)
	{
		if (m_World.Update(a_fDeltaTime))
		{
			m_World.ResetWorld();
			m_LevelManager.LoadNextLevel(m_World.GetLevel());

			m_GameEvents.OnLevelComplete();
		}
	}

	void Game::Render()
	{
		SDL_SetRenderDrawColor(&m_Renderer, 15, 20, 35, 255);
		SDL_RenderClear(&m_Renderer);

		SDL_Texture* bgTex = game::GetGame().GetResourceSystem().GetTexture(resources::ETextureType::BG);

		if (bgTex)
		{
			float bgWidth = 0.0f;
			float bgHeight = 0.0f;
			SDL_GetTextureSize(bgTex, &bgWidth, &bgHeight);

			for (float y = 0.0f; y < m_fGameHeight; y += bgHeight)
			{
				for (float x = 0.0f; x < m_fGameWidth; x += bgWidth)
				{
					SDL_FRect dstRect = { x, y, bgWidth, bgHeight };
					SDL_RenderTexture(&m_Renderer, bgTex, nullptr, &dstRect);
				}
			}
		}

		m_World.Render(m_Renderer);

		m_TextSystem.DisplayText("Lives: " + std::to_string(m_iLives), 0, 0);

		std::string levelText = "Level " + std::to_string(m_LevelManager.GetCurrentLevelIndex());
		int levelTextWidth = 0;
		int levelTextHeight = 0;
		m_TextSystem.GetTextSize(levelText, levelTextWidth, levelTextHeight);
		float textPosX = m_fGameWidth - levelTextWidth;
		float textPosY = 0;
		m_TextSystem.DisplayText(levelText, textPosX, textPosY);
	}

	const resources::ResourceSystem& Game::GetResourceSystem() const
	{
		return m_ResourceSystem;
	}

	resources::ResourceSystem& Game::GetResourceSystem()
	{
		return m_ResourceSystem;
	}

    const audio::AudioSystem& Game::GetAudioSystem() const
    {
		return m_AudioSystem;
    }

	audio::AudioSystem& Game::GetAudioSystem()
	{
		return m_AudioSystem;
	}

    const text::TextSystem& Game::GetTextSystem() const
    {
		return m_TextSystem;
    }

	text::TextSystem& Game::GetTextSystem()
	{
		return m_TextSystem;
	}

	const level::LevelManager& Game::GetLevelManager() const
	{
		return m_LevelManager;
	}

	level::LevelManager& Game::GetLevelManager()
	{
		return m_LevelManager;
	}

	const game::World& Game::GetWorld() const
	{
		return m_World;
	}

	game::World& Game::GetWorld()
	{
		return m_World;
	}

	void Game::ResetLives()
	{
		m_iLives = STARTING_LIVES;
	}

	void Game::LoseLife()
	{
		m_iLives--;
		if (m_iLives <= 0)
		{
			m_World.ResetWorld();
			m_LevelManager.LoadLevel(0, m_World.GetLevel());
			ResetLives();

			m_GameEvents.OnGameOver();
		}
		else
		{
			m_GameEvents.OnLifeLost();
		}
	}

	void Game::AddLife()
	{
		if (m_iLives < 99)
		{
			m_iLives++;

			m_GameEvents.OnLifeAdded();
		}
	}

	const GameEvents& Game::GetGameEvents() const
	{
		return m_GameEvents;
	}

	GameEvents& Game::GetGameEvents()
	{
		return m_GameEvents;
	}
}