#include "./Level.h"

#include <SDL3/SDL_render.h>

#include "arkanoid_lib/level/BrickTypes.h"
#include "arkanoid_lib/level/LevelData.h"
#include "arkanoid_lib/resources/ResourceSystem.h"

#include "arkanoid_game/game/Game.h"

namespace arkanoid::level
{
    bool Level::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
    {
		m_pBrickTexture = a_ResourceSystem.GetTexture(resources::ETextureType::BRICK);

        return true;
    }

    bool Level::Initialize(const LevelData& a_LevelData)
	{
		const int numBrickTypes = static_cast<int>(level::EBrickType::MAX);

		float textureWidth = 0.0f;
		if (!SDL_GetTextureSize(m_pBrickTexture, &textureWidth, &m_fBrickHeight))
		{
			const game::Game& game = game::GetGame();
			m_fBrickWidth = game.GetDummyWidth();
			m_fBrickHeight = game.GetDummyHeight();
		}
		else
		{
			m_fBrickWidth = textureWidth / numBrickTypes;
		}

		resources::ResourceSystem& resourceSystem = game::GetGame().GetResourceSystem();

		m_aBricks.reserve(a_LevelData.GetBricks().size());
		size_t numBricks = a_LevelData.GetBricks().size();
		for (size_t i = 0; i < numBricks; i++)
		{
			const EBrickType brickType = a_LevelData.GetBricks()[i];
			if (brickType == EBrickType::EMPTY)
			{
				continue;
			}

			// Both of these are index positions, not world positions.
			const size_t x = i % LEVEL_WIDTH;
			const size_t y = i / LEVEL_WIDTH;

			m_aBricks.emplace_back(std::make_unique<game::Brick>(brickType, static_cast<float>(x), static_cast<float>(y), m_fBrickWidth, m_fBrickHeight));

			game::Brick& brick = *m_aBricks.back();
			brick.LoadTextures(resourceSystem);
			brick.LoadSounds(resourceSystem);
		}

		return true;
	}

	void Level::Clear()
	{
		for (const std::unique_ptr<game::Brick>& brick : m_aBricks)
		{
			if (!brick)
			{
				continue;
			}

			brick->Destroy();
		}

		m_aBricks.clear();
	}

	bool Level::IsComplete() const
	{
		for (const std::unique_ptr<game::Brick>& brick : m_aBricks)
		{
			if (brick && !brick->IsDestroyed())
			{
				return false;
			}
		}
		return true;
	}

	std::vector<std::unique_ptr<game::Brick>>& Level::GetBricks()
	{
		return m_aBricks;
	}

	const std::vector<std::unique_ptr<game::Brick>>& Level::GetBricks() const
	{
		return m_aBricks;
	}

	void Level::Update(float a_fDeltaTime)
	{
		std::erase_if(m_aBricks, [](const std::unique_ptr<game::Brick>& brick)
		{
			return brick->IsDestroyed();
		});

		for (const std::unique_ptr<game::Brick>& brick : m_aBricks)
		{
			if (!brick)
			{
				continue;
			}

			brick->Update(a_fDeltaTime);
		}
	}

    void Level::Render(SDL_Renderer& a_Renderer) const
    {
		for (const std::unique_ptr<game::Brick>& brick : m_aBricks)
		{
			if (!brick)
			{
				continue;
			}

			brick->Render(a_Renderer);
		}
	}
}