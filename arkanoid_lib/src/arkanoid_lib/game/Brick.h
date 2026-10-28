#pragma once

#include <cstdint>

#include "arkanoid_lib/game/IBounceable.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace arkanoid::resources
{
	class ResourceSystem;
	class Sound;
}
namespace arkanoid::level
{
	enum class EBrickType : uint8_t;
}
namespace arkanoid::game
{
	class Brick : public IBounceable
	{
	public:
		Brick(level::EBrickType a_eType, int a_iPosX, int a_iPosY, float a_fBrickWidth, float a_fBrickHeight);

		void LoadTextures(resources::ResourceSystem& a_ResourceSystem);
		void LoadSounds(resources::ResourceSystem& a_ResourceSystem);

		SDL_FRect GetCollider() const override;

		void OnHit() override;
		void TakeDamage(uint32_t a_iDamage);

		void SetDestructible();
		void SetHealth(int32_t a_iHealth);

		bool IsDestroyed() const;

		int GetPosX() const;
		int GetPosY() const;

		level::EBrickType GetBrickType() const;

		void Update(float m_fDeltaTime);
		void Render(SDL_Renderer& a_Renderer) const;
	private:
		level::EBrickType m_eType;
		int32_t m_iHealth = 1;
		bool m_bDestructible = false;
		int m_iPosX = 0, m_iPosY = 0;

		SDL_FRect m_rCollider;

		SDL_Texture* m_pBrickTexture = nullptr; // Not owned.
		resources::Sound* m_pHitBrickSound = nullptr; // Not owned.

		SDL_Texture* m_pExplosionTexture = nullptr; // Not owned.
		int m_iExplosionIndex = 0;
		float m_fExplosionTimer = 0;
		float m_fExplosionTime = 0.1f;
		float m_iMaxExplosionFrames = 3;
		float m_fExplosionWidth = 0;
		float m_fExplosionHeight = 0;
	};
}