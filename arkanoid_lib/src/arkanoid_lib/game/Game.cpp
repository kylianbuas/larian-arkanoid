#include "./Game.h"

#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

#include "arkanoid_lib/level/LevelData.h"
#include "arkanoid_lib/resources/ResourceSystem.h"

namespace arkanoid::game
{
	Game::Game(SDL_Window& a_Window, SDL_Renderer& a_Renderer) :
		m_Window(a_Window),
		m_Renderer(a_Renderer),
		m_ResourceSystem(a_Renderer),
		m_TextSystem(a_Renderer),
		m_LevelManager(m_ResourceSystem)
	{
		SDL_Log("Working directory: %s", SDL_GetCurrentDirectory());
	}

	void Game::Initialize()
	{
		m_World.Initialize();
		m_LevelManager.LoadLevel(0, m_World.GetLevel());
		ResetLives();
	}

	constexpr float DUMMY_DEFAULT_WIDTH = 16;
	constexpr float DUMMY_DEFAULT_HEIGHT = 8;
	void Game::LoadTextures()
	{
		m_ResourceSystem.LoadTexture(resources::ETextureType::DUMMY, "tex_dummy.png"); // Used for measuring the bricks.

		SDL_Texture* m_pDummyTexture = m_ResourceSystem.GetTexture(resources::ETextureType::DUMMY);
		if (!m_pDummyTexture || !SDL_GetTextureSize(m_pDummyTexture, &m_fDummyWidth, &m_fDummyHeight))
		{
			m_fDummyWidth = DUMMY_DEFAULT_WIDTH;
			m_fDummyHeight = DUMMY_DEFAULT_HEIGHT;
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

		m_World.LoadTextures(m_ResourceSystem);
	}

	void Game::LoadSounds()
	{
		m_ResourceSystem.LoadSound(resources::ESoundType::HIT, "snd_hit.wav");
		m_ResourceSystem.LoadSound(resources::ESoundType::HIT_BRICK, "snd_hit_brick.wav");
		m_ResourceSystem.LoadSound(resources::ESoundType::LEVEL_COMPLETE, "snd_level_complete.wav");
		m_ResourceSystem.LoadSound(resources::ESoundType::LIFE_LOST, "snd_life_lost.wav");
		m_ResourceSystem.LoadSound(resources::ESoundType::GAME_OVER, "snd_game_over.wav");

		m_pLevelCompleteSound = m_ResourceSystem.GetSound(resources::ESoundType::LEVEL_COMPLETE);
		m_pLifeLostSound = m_ResourceSystem.GetSound(resources::ESoundType::LIFE_LOST);
		m_pGameOverSound = m_ResourceSystem.GetSound(resources::ESoundType::GAME_OVER);

		m_World.LoadSounds(m_ResourceSystem);
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

			if (m_pLevelCompleteSound)
			{
				m_AudioSystem.Play(*m_pLevelCompleteSound);
			}
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

	constexpr int MAX_LIVES = 3;
	void Game::ResetLives()
	{
		m_iLives = MAX_LIVES;
	}

	void Game::LoseLife()
	{
		m_iLives--;
		if (m_iLives <= 0)
		{
			m_World.ResetWorld();
			m_LevelManager.LoadLevel(0, m_World.GetLevel());
			ResetLives();

			if (m_pGameOverSound)
			{
				m_AudioSystem.Play(*m_pGameOverSound);
			}
		}
		else if (m_pLifeLostSound)
		{
			m_AudioSystem.Play(*m_pLifeLostSound);
		}
	}
}