#include "./Brick.h"

#include <SDL3/SDL_render.h>

#include "arkanoid_lib/level/BrickTypes.h"

#include "arkanoid_game/game/Explosion.h"
#include "arkanoid_game/game/Game.h"
#include "arkanoid_game/game/PowerUp.h"
#include "arkanoid_game/game/PowerUpTypes.h"
#include "arkanoid_game/utils/Random.h"

namespace arkanoid::game
{
	Brick::Brick(level::EBrickType a_eType, int a_iPosX, int a_iPosY, float a_fBrickWidth, float a_fBrickHeight) : GameObject(a_iPosX, a_iPosY), IBounceable(),
		m_eType(a_eType)
	{
		m_fPosX *= a_fBrickWidth;
		m_fPosY *= a_fBrickHeight;

		m_rCollider = {
			m_fPosX,
			m_fPosY,
			a_fBrickWidth,
			a_fBrickHeight
		};
	}

	void Brick::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
	{
		m_pBrickTexture = a_ResourceSystem.GetTexture(resources::ETextureType::BRICK);
	}

	void Brick::LoadSounds(resources::ResourceSystem& a_ResourceSystem)
	{
		m_pHitBrickSound = a_ResourceSystem.GetSound(resources::ESoundType::HIT_BRICK);
	}

	SDL_FRect Brick::GetCollider() const
	{
		return m_rCollider;
	}

	void Brick::OnHit()
	{
		TakeDamage(1);

		if (m_pHitBrickSound)
		{
			game::GetGame().GetAudioSystem().Play(*m_pHitBrickSound, 1 + ((game::GetGame().GetWorld().GetBall().GetCombo() - 1) * 0.15f));
		}
	}

	void Brick::TakeDamage(uint32_t a_iDamage)
	{
		if (m_iHealth <= 0)
		{
			return;
		}

		m_iHealth -= a_iDamage;

		if (m_iHealth <= 0)
		{
			Destroy();
		}
	}

	void Brick::OnDestroyed()
	{
		const SDL_FRect collider = GetCollider();
		game::GetGame().GetWorld().SpawnGameObject<Explosion>(
			collider.x + collider.w * 0.5f,
			collider.y + collider.h * 0.5f
		);

		if (utils::Random::RollChance(GetGame().GetPowerUpSpawnChance()))
		{
			game::GetGame().GetWorld().SpawnGameObject<PowerUp>(
				EPowerUpType::ONE_UP,
				collider.x + collider.w * 0.5f,
				collider.y + collider.h * 0.5f
			);
		}
	}

	void Brick::SetDestructible()
	{
		m_bDestructible = true;
	}

	void Brick::SetHealth(int32_t a_iHealth)
	{
		m_iHealth = a_iHealth;
	}

	level::EBrickType Brick::GetBrickType() const
	{
		return m_eType;
	}

	void Brick::Update(float a_fDeltaTime)
	{
	}

	void Brick::Render(SDL_Renderer& a_Renderer) const
	{
		const SDL_FRect collider = GetCollider();

		if (m_pBrickTexture)
		{
			SDL_FRect srcRect = {
				static_cast<float>((static_cast<int>(m_eType) - 1) * collider.w),
				0.0f,
				static_cast<float>(collider.w),
				static_cast<float>(collider.h)
			};

			SDL_RenderTexture(&a_Renderer, m_pBrickTexture, &srcRect, &collider);
		}
		else
		{
			Uint8 r, g, b;
			level::GetBrickColor(m_eType, r, g, b);
			SDL_SetRenderDrawColor(&a_Renderer, r, g, b, 255);
			SDL_RenderFillRect(&a_Renderer, &collider);
		}
	}
}