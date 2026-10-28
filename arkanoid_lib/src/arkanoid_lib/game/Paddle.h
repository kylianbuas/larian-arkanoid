#pragma once

#include <SDL3/SDL_rect.h>

#include "arkanoid_lib/game/IBounceable.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace arkanoid::resources
{
	class ResourceSystem;
}
namespace arkanoid::game
{
	class Paddle : public IBounceable
	{
	public:
		Paddle();

		void LoadTextures(resources::ResourceSystem& a_ResourceSystem);
		void Reset();

		SDL_FRect GetCollider() const override;
		bool Bounce(float a_fOverlapX, float a_fOverlapY, float& a_fDirectionX, float& a_fDirectionY) override;
		void OnHit() override;

		void Update(float a_fDeltaTime);
		void Render(SDL_Renderer& a_Renderer) const;
	private:
		SDL_Texture* m_pPaddleTexture = nullptr; // Not owned.

		float m_fPaddleWidth = 0;
		float m_fPaddleHeight = 0;

		float m_fPosX = 0.5f;
		float m_fPosY = 0;

		bool m_bIsMoving = false;
		float m_fMovementSpeed = 1.f;
	};
}