#include "./Brick.h"

#include <SDL3/SDL_render.h>

#include "arkanoid_lib/game/Game.h"
#include "arkanoid_lib/level/BrickTypes.h"

namespace arkanoid::game
{
	Brick::Brick(level::EBrickType a_eType, int a_iPosX, int a_iPosY, float a_fBrickWidth, float a_fBrickHeight) : IBounceable(),
		m_eType(a_eType),
		m_iPosX(a_iPosX),
		m_iPosY(a_iPosY)
	{
		m_rCollider = {
			m_iPosX * a_fBrickWidth,
				m_iPosY * a_fBrickHeight,
				a_fBrickWidth,
				a_fBrickHeight
		};

		game::GetGame().GetWorld().AddBounceable(*this);
	}

	void Brick::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
	{
		m_pBrickTexture = a_ResourceSystem.GetTexture(resources::ETextureType::BRICK);
		m_pExplosionTexture = a_ResourceSystem.GetTexture(resources::ETextureType::EXPLOSION);

		SDL_GetTextureSize(m_pExplosionTexture, &m_fExplosionWidth, &m_fExplosionHeight);
		m_fExplosionWidth /= m_iMaxExplosionFrames;
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
			game::GetGame().GetWorld().RemoveBounceable(*this);
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

	bool Brick::IsDestroyed() const
	{
		return m_iHealth <= 0;
	}

	int Brick::GetPosX() const
	{
		return m_iPosX;
	}

	int Brick::GetPosY() const
	{
		return m_iPosY;
	}

	level::EBrickType Brick::GetBrickType() const
	{
		return m_eType;
	}

	void Brick::Update(float m_fDeltaTime)
	{
		if (IsDestroyed())
		{
			if (m_pExplosionTexture)
			{
				m_fExplosionTimer += m_fDeltaTime;

				if (m_fExplosionTimer >= m_fExplosionTime)
				{
					m_fExplosionTimer -= m_fExplosionTime;
					m_iExplosionIndex++;

					if (m_iExplosionIndex >= m_iMaxExplosionFrames)
					{
						m_pExplosionTexture = nullptr;
					}
				}
			}
		}
	}

	void Brick::Render(SDL_Renderer& a_Renderer) const
	{
		const SDL_FRect collider = GetCollider();

		if (IsDestroyed())
		{
			SDL_FRect srcRect = {
				m_iExplosionIndex * m_fExplosionWidth,
				0.0f,
				m_fExplosionWidth,
				m_fExplosionHeight
			};

			SDL_FRect dstRect = {
				m_rCollider.x + (m_rCollider.w - m_fExplosionWidth) * 0.5f,
				m_rCollider.y + (m_rCollider.h - m_fExplosionHeight) * 0.5f,
				m_fExplosionWidth,
				m_fExplosionHeight
			};

			SDL_RenderTexture(&a_Renderer, m_pExplosionTexture, &srcRect, &dstRect);
		}
		else
		{
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
				switch (m_eType)
				{
					case level::EBrickType::RED:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 255, 0, 0, 255);
						break;
					}
					case level::EBrickType::BLUE:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 0, 0, 255, 255);
						break;
					}
					case level::EBrickType::GREEN:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 0, 255, 0, 255);
						break;
					}
					case level::EBrickType::YELLOW:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 255, 216, 0, 255);
						break;
					}
					case level::EBrickType::ORANGE:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 255, 106, 0, 255);
						break;
					}
					case level::EBrickType::PURPLE:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 178, 0, 255, 255);
						break;
					}
					case level::EBrickType::CYAN:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 0, 255, 255, 255);
						break;
					}
					case level::EBrickType::WHITE:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 25, 255, 255, 255);
						break;
					}
					case level::EBrickType::RAINBOW:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 50, 50, 255, 255);
						break;
					}
					case level::EBrickType::QUESTIONMARK:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 255, 216, 0, 255);
						break;
					}
					case level::EBrickType::BRICK:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 151, 94, 45, 255);
						break;
					}
					case level::EBrickType::GROUND:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 185, 99, 55, 255);
						break;
					}
					case level::EBrickType::BLACK:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 0, 0, 0, 255);
						break;
					}
					case level::EBrickType::GRAY:
					{
						SDL_SetRenderDrawColor(&a_Renderer, 94, 94, 94, 255);
						break;
					}
				}
				SDL_RenderFillRect(&a_Renderer, &collider);
			}
		}
	}
}