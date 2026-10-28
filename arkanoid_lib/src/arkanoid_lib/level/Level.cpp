#include "./Level.h"

#include <SDL3/SDL_render.h>

#include "arkanoid_lib/game/Game.h"
#include "arkanoid_lib/level/BrickTypes.h"
#include "arkanoid_lib/level/LevelData.h"
#include "arkanoid_lib/resources/ResourceSystem.h"

namespace arkanoid::level
{
    bool Level::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
    {
		m_pBrickTexture = a_ResourceSystem.GetTexture(resources::ETextureType::BRICK);
		m_pBrickShineTexture = a_ResourceSystem.GetTexture(resources::ETextureType::BRICK_SHINE_OVERLAY);

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

			const int x = i % LEVEL_WIDTH;
			const int y = i / LEVEL_WIDTH;

			game::Brick& brick = m_aBricks.emplace_back(brickType, x, y, m_fBrickWidth, m_fBrickHeight);

			brick.LoadTextures(resourceSystem);
			brick.LoadSounds(resourceSystem);
		}

		return true;
	}

	void Level::Clear()
	{
		m_aBricks.clear();
	}

	bool Level::IsComplete() const
	{
		for (const game::Brick& brick : m_aBricks)
		{
			if (!brick.IsDestroyed())
			{
				return false;
			}
		}
		return true;
	}

	std::vector<game::Brick>& Level::GetBricks()
	{
		return m_aBricks;
	}

	const std::vector<game::Brick>& Level::GetBricks() const
	{
		return m_aBricks;
	}

	void Level::Shine(float a_fDeltaTime)
	{
		if (m_fShineDelayTime > 0.0f)
		{
			m_fShineDelayTime -= a_fDeltaTime;
			if (m_fShineDelayTime > 0.0f)
			{
				return;
			}

			m_fShineTime = -m_fShineDelayTime;
			m_fShineDelayTime = 0.0f;
		}
		else
		{
			m_fShineTime += a_fDeltaTime;
		}

		if (m_fShineTime < m_fShineChange)
		{
			return;
		}

		m_fShineTime -= m_fShineChange;
		m_iShineIndex++;

		if (m_iShineIndex >= m_iShineMax)
		{
			m_iShineIndex = 0;
			m_fShineDelayTime = m_fShineDelay;
		}
	}

	void Level::Update(float a_fDeltaTime)
	{
		size_t bricksSize = m_aBricks.size();
		for (size_t i = 0; i < bricksSize; i++)
		{
			game::Brick& brick = m_aBricks[i];
			brick.Update(a_fDeltaTime);
		}

		Shine(a_fDeltaTime);
	}

    void Level::Render(SDL_Renderer& a_Renderer) const
    {
		size_t bricksSize = m_aBricks.size();
		for (size_t i = 0; i < bricksSize; i++)
		{
			const game::Brick& brick = m_aBricks[i];

			brick.Render(a_Renderer);

			if (brick.IsDestroyed())
			{
				continue;
			}

			if (m_pBrickShineTexture)
			{
				const SDL_FRect collider = brick.GetCollider();
				const SDL_FRect shineSrcRect = {
					static_cast<float>(m_iShineIndex * collider.w),
					0.0f,
					static_cast<float>(collider.w),
					static_cast<float>(collider.h)
				};
				SDL_RenderTexture(&a_Renderer, m_pBrickShineTexture, &shineSrcRect, &collider);
			}
		}
	}
}