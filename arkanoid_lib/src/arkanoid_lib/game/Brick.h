#pragma once

#include <cstdint>

#include "arkanoid_lib/game/GameObject.h"
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
	constexpr float DUMMY_DEFAULT_WIDTH = 16;
	constexpr float DUMMY_DEFAULT_HEIGHT = 8;

	class Brick : public GameObject, public IBounceable
	{
	public:
		Brick(level::EBrickType a_eType, int a_iPosX, int a_iPosY, float a_fBrickWidth, float a_fBrickHeight);
		virtual ~Brick() = default;

		void LoadTextures(resources::ResourceSystem& a_ResourceSystem);
		void LoadSounds(resources::ResourceSystem& a_ResourceSystem);

		SDL_FRect GetCollider() const override;

		void OnHit() override;
		void TakeDamage(uint32_t a_iDamage);

		void OnDestroyed() override;

		void SetDestructible();
		void SetHealth(int32_t a_iHealth);

		level::EBrickType GetBrickType() const;

		void Update(float a_fDeltaTime) override;
		void Render(SDL_Renderer& a_Renderer) const override;
	private:
		level::EBrickType m_eType;
		int32_t m_iHealth = 1;
		bool m_bDestructible = false;

		SDL_FRect m_rCollider;

		SDL_Texture* m_pBrickTexture = nullptr; // Not owned.
		resources::Sound* m_pHitBrickSound = nullptr; // Not owned.
	};
}