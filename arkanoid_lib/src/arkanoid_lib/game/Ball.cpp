#include "./Ball.h"

#include <SDL3/SDL_render.h>

#include "arkanoid_lib/game/Game.h"

namespace arkanoid::game
{
	void Ball::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
	{
		m_pBallTexture = a_ResourceSystem.GetTexture(resources::ETextureType::BALL);
		if (!m_pBallTexture || !SDL_GetTextureSize(m_pBallTexture, &m_fBallWidth, &m_fBallHeight))
		{
			const game::Game& game = game::GetGame();
			m_fBallWidth = game.GetDummyHeight();
			m_fBallHeight = game.GetDummyHeight();
		}
	}

	void Ball::LoadSounds(resources::ResourceSystem& a_ResourceSystem)
    {
		m_pHitSound = a_ResourceSystem.GetSound(resources::ESoundType::HIT);
    }

	void Ball::Reset()
	{
		m_bLaunched = false;
		m_fDirectionX = 0.0f;
		m_fDirectionY = -1.0f;
		m_iCombo = 0;
	}

	SDL_FRect Ball::GetCollider() const
	{
		SDL_FRect collider = {
			m_fPosX,
			m_fPosY,
			m_fBallWidth,
			m_fBallHeight
		};

		return collider;
	}

	size_t Ball::GetCombo() const
	{
		return m_iCombo;
	}

	void Ball::ResetCombo()
	{
		m_iCombo = 0;
	}

	void Ball::Bounce(float a_fDirectionX, float a_fDirectionY)
	{
		m_fDirectionX = a_fDirectionX;
		m_fDirectionY = a_fDirectionY;
	}

	float Ball::GetDirectionX() const
	{
		return m_fDirectionX;
	}

	float Ball::GetDirectionY() const
	{
		return m_fDirectionY;
	}

	void Ball::Update(float a_fDeltaTime)
	{
		game::Game& game = game::GetGame();

		if (!m_bLaunched)
		{
			const SDL_FRect paddleCollider = game.GetWorld().GetPaddle().GetCollider();

			m_fPosX = paddleCollider.x + (paddleCollider.w - m_fBallWidth) * 0.5f;
			m_fPosY = paddleCollider.y - (m_fBallHeight * 2);

			const bool* keyboardState = SDL_GetKeyboardState(nullptr);

			if (keyboardState[SDL_SCANCODE_SPACE])
			{
				m_bLaunched = true;

				m_fDirectionX = 0.5f;
				m_fDirectionY = -0.7f;

				const float directionLength = std::sqrtf(m_fDirectionX * m_fDirectionX + m_fDirectionY * m_fDirectionY);
				m_fDirectionX /= directionLength;
				m_fDirectionY /= directionLength;
			}
		}

		if (!m_bLaunched)
		{
			return;
		}

		bool hitAnything = false;

		m_fPosX += m_fDirectionX * m_fMovementSpeed * a_fDeltaTime;
		m_fPosY += m_fDirectionY * m_fMovementSpeed * a_fDeltaTime;

		const float gameWidth = game.GetGameWidth();
		if (m_fPosX <= 0.0f)
		{
			m_fPosX = 0.0f;
			Bounce(-m_fDirectionX, m_fDirectionY);

			hitAnything = true;
		}
		else if (m_fPosX + m_fBallWidth >= gameWidth)
		{
			m_fPosX = gameWidth - m_fBallWidth;
			Bounce(-m_fDirectionX, m_fDirectionY);

			hitAnything = true;
		}

		if (m_fPosY <= 0.0f)
		{
			m_fPosY = 0.0f;
			Bounce(m_fDirectionX, -m_fDirectionY);

			hitAnything = true;
		}

		if (m_fPosY >= game.GetGameHeight())
		{
			game.LoseLife();
			m_bLaunched = false;

			ResetCombo();

			return;
		}

		std::vector<IBounceable*>& bounceables = game.GetWorld().GetBounceables();
		const size_t bounceablesSize = bounceables.size();

		for (int i = static_cast<int>(bounceablesSize) - 1; i >= 0; i--)
		{
			IBounceable* bounceable = bounceables[i];
			if (!bounceable)
			{
				bounceables.erase(bounceables.begin() + i);
				continue;
			}

			const SDL_FRect ballRect = GetCollider();
			const SDL_FRect bounceableRect = bounceable->GetCollider();

			if (!SDL_HasRectIntersectionFloat(&ballRect, &bounceableRect))
			{
				continue;
			}

			const float overlapLeft = ballRect.x + ballRect.w - bounceableRect.x;
			const float overlapRight = bounceableRect.x + bounceableRect.w - ballRect.x;
			const float overlapTop = ballRect.y + ballRect.h - bounceableRect.y;
			const float overlapBottom = bounceableRect.y + bounceableRect.h - ballRect.y;

			const float overlapX = overlapLeft < overlapRight ? overlapLeft : overlapRight;
			const float overlapY = overlapTop < overlapBottom ? overlapTop : overlapBottom;
			
			float directionX = m_fDirectionX;
			float directionY = m_fDirectionY;

			if (overlapX < overlapY)
			{
				m_fPosX += ballRect.x < bounceableRect.x ? -overlapX : overlapX;
			}
			else
			{
				m_fPosY += ballRect.y < bounceableRect.y ? -overlapY : overlapY;
			}

			if (!bounceable->Bounce(overlapX, overlapY, directionX, directionY))
			{
				if (overlapX < overlapY)
				{
					directionX = -m_fDirectionX;
					directionY = m_fDirectionY;
				}
				else
				{
					directionX = m_fDirectionX;
					directionY = -m_fDirectionY;
				}
			}

			m_iCombo++;

			Bounce(directionX, directionY);
			bounceable->OnHit();

			hitAnything = true;

			break;
		}

		if (hitAnything && m_pHitSound)
		{
			game::GetGame().GetAudioSystem().Play(*m_pHitSound);
		}
	}

	void Ball::Render(SDL_Renderer& a_Renderer) const
	{
		const SDL_FRect collider = GetCollider();
		if (m_pBallTexture)
		{
			SDL_RenderTexture(&a_Renderer, m_pBallTexture, nullptr, &collider);
		}
		else
		{
			SDL_SetRenderDrawColor(&a_Renderer, 94, 94, 94, 255);
			SDL_RenderFillRect(&a_Renderer, &collider);
		}
	}
}