#include "./Paddle.h"

#include <SDL3/SDL_render.h>

#include "arkanoid_lib/level/BrickTypes.h"

#include "arkanoid_game/game/Game.h"
#include "arkanoid_game/game/GameSettings.h"
#include "arkanoid_game/game/objects/Laser.h"

namespace arkanoid::game
{
    Paddle::Paddle() : IGameObject(0, 0), IBounceable()
    {}

	void Paddle::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
	{
		m_pPaddleTexture = game::GetGame().GetResourceSystem().GetTexture(resources::ETextureType::PADDLE);

		const game::Game& game = game::GetGame();

		float textureWidth = 0.0f;
		float textureHeight = 0.0f;
		if (!m_pPaddleTexture || !SDL_GetTextureSize(m_pPaddleTexture, &textureWidth, &textureHeight))
		{
			m_fPaddleWidth = game.GetDummyWidth() * 2;
			m_fPaddleHeight = game.GetDummyHeight();
		}
		else
		{
			m_fPaddleWidth = textureWidth / 2;
			m_fPaddleHeight = textureHeight;
		}

		m_fPosY = game::GetGame().GetDummyHeight()* level::PADDLE_POS_Y;
	}

    void Paddle::Reset()
    {
		game::GetGame().GetWorld().AddBounceable(*this);
		m_bIsMoving = false;

		m_fShootingTimer = 0.0f;
		m_fShootingDelay = 0.0f;
	}

	SDL_FRect Paddle::GetCollider() const
	{
		// We need the game width and height here because the pos is from 0 to 1.
		return {
			(game::GetGame().GetGameWidth() - m_fPaddleWidth) * m_fPosX,
			m_fPosY,
			m_fPaddleWidth,
			m_fPaddleHeight
		};
	}

    bool Paddle::Bounce(float a_fOverlapX, float a_fOverlapY, float& a_fDirectionX, float& a_fDirectionY)
    {
		const SDL_FRect paddleRect = GetCollider();
		const SDL_FRect ballRect = game::GetGame().GetWorld().GetBall().GetCollider();

		const float ballCenterX = ballRect.x + ballRect.w * 0.5f;
		const float paddleCenterX = paddleRect.x + paddleRect.w * 0.5f;
		const float hitPosition = (ballCenterX - paddleCenterX) / (paddleRect.w * 0.5f);

		const float maxBounceAngle = 60.0f * SDL_PI_D / 180.0f;
		const float bounceAngle = hitPosition * maxBounceAngle;

		a_fDirectionX = sinf(bounceAngle);
		a_fDirectionY = -cosf(bounceAngle);

		return true;
    }

	void Paddle::OnHit()
	{
		game::GetGame().GetWorld().GetBall().ResetCombo();
	}

	void Paddle::Update(float a_fDeltaTime)
	{
		const bool* keyboardState = SDL_GetKeyboardState(nullptr);

		if (m_fShootingTimer > 0.0f)
		{
			if (m_fShootingDelay > 0.0f)
			{
				m_fShootingDelay -= a_fDeltaTime;
			}
			m_fShootingTimer -= a_fDeltaTime;

			if (keyboardState[SDL_SCANCODE_SPACE] && m_fShootingDelay <= 0)
			{
				const SDL_FRect collider = GetCollider();

				m_fShootingDelay = SHOOTING_POWER_UP_DELAY;
				GetGame().GetWorld().SpawnGameObject<Laser>(collider.x + (collider.w / 2), collider.y - level::DUMMY_DEFAULT_HEIGHT);

				GetGame().GetGameEvents().OnShoot();
			}
		}

		m_bIsMoving = false;

		if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT])
		{
			m_bIsMoving = true;
			m_fPosX -= m_fMovementSpeed * a_fDeltaTime;
		}
		if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT])
		{
			m_bIsMoving = true;
			m_fPosX += m_fMovementSpeed * a_fDeltaTime;
		}

		if (m_fPosX < 0.0f)
		{
			m_fPosX = 0.0f;
		}
		else if (m_fPosX > 1.0f)
		{
			m_fPosX = 1.0f;
		}
	}

	void Paddle::Render(SDL_Renderer& a_Renderer) const
	{
		const SDL_FRect collider = GetCollider();

		if (m_pPaddleTexture)
		{
			SDL_FRect srcRect = {
				m_fPaddleWidth * static_cast<float>(m_bIsMoving),
				0.0f,
				m_fPaddleWidth,
				m_fPaddleHeight
			};

			SDL_RenderTexture(&a_Renderer, m_pPaddleTexture, &srcRect, &collider);
		}
		else
		{
			SDL_SetRenderDrawColor(&a_Renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(&a_Renderer, &collider);
		}
	}

    void Paddle::ActivateShooting()
    {
		m_fShootingTimer = SHOOTING_POWER_UP_TIMER;
		m_fShootingDelay = 0.0f;
	}
}