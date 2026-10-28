#pragma once

#include <SDL3/SDL_rect.h>

struct SDL_Renderer;
struct SDL_Texture;

namespace arkanoid::resources
{
	class ResourceSystem;
	class Sound;
}
namespace arkanoid::game
{
	class Ball
	{
	public:
		void LoadTextures(resources::ResourceSystem& a_ResourceSystem);
		void LoadSounds(resources::ResourceSystem& a_ResourceSystem);
		void Reset();

		void Bounce(float a_fDirectionX, float a_fDirectionY);

		float GetDirectionX() const;
		float GetDirectionY() const;

		SDL_FRect GetCollider() const;

		size_t GetCombo() const;

		void ResetCombo();

		void Update(float a_fDeltaTime);

		void Render(SDL_Renderer& a_Renderer) const;
	private:
		bool m_bLaunched = false;

		resources::Sound* m_pHitSound = nullptr; // Not owned.
		SDL_Texture* m_pBallTexture = nullptr; // Not owned.

		float m_fBallWidth = 0;
		float m_fBallHeight = 0;

		float m_fPosX = 0.0f;
		float m_fPosY = 0.0f;

		float m_fMovementSpeed = 250.0f;
		float m_fDirectionX = 0.0f;
		float m_fDirectionY = 0.0f;

		size_t m_iCombo = 0;
	};
}