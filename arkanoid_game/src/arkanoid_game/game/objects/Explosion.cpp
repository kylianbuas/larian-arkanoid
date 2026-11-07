#include "./Explosion.h"

#include <SDL3/SDL_render.h>

#include "arkanoid_game/game/Game.h"

namespace arkanoid::game
{
	Explosion::Explosion(float a_fPosX, float a_fPosY) : IGameObject(a_fPosX, a_fPosY)
	{
		m_pExplosionTexture = game::GetGame().GetResourceSystem().GetTexture(resources::ETextureType::EXPLOSION);

		SDL_GetTextureSize(m_pExplosionTexture, &m_fExplosionWidth, &m_fExplosionHeight);
		m_fExplosionWidth /= m_iMaxExplosionFrames;
	}

	void Explosion::Update(float a_fDeltaTime)
	{
		if (m_pExplosionTexture)
		{
			m_fExplosionTimer += a_fDeltaTime;

			if (m_fExplosionTimer >= m_fExplosionTime)
			{
				m_fExplosionTimer -= m_fExplosionTime;
				m_iExplosionIndex++;

				if (m_iExplosionIndex >= m_iMaxExplosionFrames)
				{
					Destroy();
				}
			}
		}
		else
		{
			Destroy();
		}
	}

	void Explosion::Render(SDL_Renderer& a_Renderer) const
	{
		SDL_FRect srcRect = {
			m_iExplosionIndex * m_fExplosionWidth,
			0.0f,
			m_fExplosionWidth,
			m_fExplosionHeight
		};

		SDL_FRect dstRect = {
			m_fPosX - m_fExplosionWidth * 0.5f,
			m_fPosY - m_fExplosionHeight * 0.5f,
			m_fExplosionWidth,
			m_fExplosionHeight
		};

		SDL_RenderTexture(&a_Renderer, m_pExplosionTexture, &srcRect, &dstRect);
	}
}