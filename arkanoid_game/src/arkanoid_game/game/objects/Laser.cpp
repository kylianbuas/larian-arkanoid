#include "./Laser.h"

#include <SDL3/SDL_render.h>

#include "arkanoid_game/game/Game.h"
#include "arkanoid_game/game/GameSettings.h"

namespace arkanoid::game
{
	Laser::Laser(float a_fPosX, float a_fPosY) : IGameObject(a_fPosX, a_fPosY)
	{
		m_pLaserTexture = game::GetGame().GetResourceSystem().GetTexture(resources::ETextureType::LASER);

		if (!SDL_GetTextureSize(m_pLaserTexture, &m_fLaserWidth, &m_fLaserHeight))
		{
			const game::Game& game = game::GetGame();
			m_fLaserWidth = game.GetDummyWidth();
			m_fLaserHeight = game.GetDummyHeight();
		}
	}

	SDL_FRect Laser::GetCollider() const
	{
		return {
			m_fPosX - m_fLaserWidth * 0.5f,
			m_fPosY - m_fLaserHeight * 0.5f,
			m_fLaserWidth,
			m_fLaserHeight
		};
	}

	void Laser::Update(float a_fDeltaTime)
	{
		m_fPosY -= (LASER_SPEED * a_fDeltaTime);

		if (m_fPosY <= 0.0f)
		{
			Destroy();
		}

		const Game& game = GetGame();
		const std::vector<IBounceable*>& bounceables = game.GetWorld().GetBounceables();
		const size_t bounceablesSize = bounceables.size();

		for (int i = static_cast<int>(bounceablesSize) - 1; i >= 0; i--)
		{
			IBounceable* bounceable = bounceables[i];
			if (!bounceable)
			{
				continue;
			}

			const SDL_FRect ballRect = GetCollider();
			const SDL_FRect bounceableRect = bounceable->GetCollider();

			if (!SDL_HasRectIntersectionFloat(&ballRect, &bounceableRect))
			{
				continue;
			}

			bounceable->OnHit();
			Destroy();

			break;
		}
	}

	void Laser::Render(SDL_Renderer& a_Renderer) const
	{
		const SDL_FRect collider = GetCollider();

		if (m_pLaserTexture)
		{
			SDL_FRect srcRect = {
				0.0f,
				0.0f,
				m_fLaserWidth,
				m_fLaserHeight
			};

			SDL_RenderTexture(&a_Renderer, m_pLaserTexture, &srcRect, &collider);
		}
		else
		{
			SDL_SetRenderDrawColor(&a_Renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(&a_Renderer, &collider);
		}
	}
}