#pragma once

#include <cstdint>

#include "arkanoid_game/game/IGameObject.h"
#include "arkanoid_game/game/IBounceable.h"

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
{;
	class Brick : public IGameObject, public IBounceable
	{
	public:
		Brick(level::EBrickType a_eType, float a_iPosX, float a_iPosY, float a_fBrickWidth, float a_fBrickHeight);
		virtual ~Brick() = default;

		void LoadTextures(resources::ResourceSystem& a_ResourceSystem);

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
	};
}