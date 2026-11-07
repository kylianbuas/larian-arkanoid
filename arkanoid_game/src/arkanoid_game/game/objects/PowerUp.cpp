#include "./PowerUp.h"

#include <SDL3/SDL_render.h>

#include "arkanoid_game/game/Game.h"
#include "arkanoid_game/game/GameSettings.h"
#include "arkanoid_game/game/PowerUpTypes.h"

namespace arkanoid::game
{
	PowerUp::PowerUp(EPowerUpType a_eEPowerUpType, float a_fPosX, float a_fPosY) : IGameObject(a_fPosX, a_fPosY), IConsumeable(),
		m_eEPowerUpType(a_eEPowerUpType)
	{
		m_pPowerUpTexture = game::GetGame().GetResourceSystem().GetTexture(resources::ETextureType::POWER_UP);

		SDL_GetTextureSize(m_pPowerUpTexture, &m_fPowerUpWidth, &m_fPowerUpHeight);
		m_fPowerUpWidth /= static_cast<float>(EPowerUpType::MAX);
	}

	SDL_FRect PowerUp::GetCollider() const
	{
		return {
			m_fPosX - m_fPowerUpWidth * 0.5f,
			m_fPosY - m_fPowerUpHeight * 0.5f,
			m_fPowerUpWidth,
			m_fPowerUpHeight
		};
	}

	bool PowerUp::Consume()
	{
		if (!m_bCanConsume)
		{
			return false;
		}
		
		// For simplicity, a simple switch case for power-ups was fine, but if power-ups move and behave
		// more different than now, there might need to be some sort of factory.
		Game& game = GetGame();
		switch (m_eEPowerUpType)
		{
			case EPowerUpType::ONE_UP:
			{
				game.AddLife();
				break;
			}
		}

		Destroy();
		return true;
	}

	void PowerUp::Update(float a_fDeltaTime)
	{
		m_fPosY += (POWER_UP_FALL_SPEED * a_fDeltaTime);

		if (!m_bCanConsume)
		{
			m_fConsumeTimer += a_fDeltaTime;

			if (m_fConsumeTimer >= m_fConsumeTime)
			{
				m_bCanConsume = true;
			}
		}

		if (m_fPosY >= GetGame().GetGameHeight())
		{
			Destroy();
		}
	}

	void PowerUp::Render(SDL_Renderer& a_Renderer) const
	{
		const SDL_FRect collider = GetCollider();
		const Uint8 alpha = m_bCanConsume ? 255 : 127;

		if (m_pPowerUpTexture)
		{
			SDL_FRect srcRect = {
				0.0f,
				0.0f,
				m_fPowerUpWidth,
				m_fPowerUpHeight
			};

			SDL_SetTextureAlphaMod(m_pPowerUpTexture, alpha);
			SDL_RenderTexture(&a_Renderer, m_pPowerUpTexture, &srcRect, &collider);
			SDL_SetTextureAlphaMod(m_pPowerUpTexture, 255);
		}
		else
		{
			Uint8 r, g, b;
			GetPowerUpColor(m_eEPowerUpType, r, g, b);

			SDL_SetRenderDrawColor(&a_Renderer, r, g, b, alpha);
			SDL_RenderFillRect(&a_Renderer, &collider);
		}
	}
}