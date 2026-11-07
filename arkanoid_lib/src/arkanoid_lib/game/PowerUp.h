#pragma once

#include <cstdint>

#include "arkanoid_lib/game/IConsumeable.h"
#include "arkanoid_lib/game/GameObject.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace arkanoid::resources
{
	class ResourceSystem;
	class Sound;
}
namespace arkanoid::game
{
	enum class EPowerUpType : uint8_t;

	constexpr float POWER_UP_FALL_SPEED = 0.025f;

	class PowerUp : public GameObject, public IConsumeable
	{
	public:
		PowerUp(EPowerUpType a_eEPowerUpType, float a_fPosX, float a_fPosY);
		virtual ~PowerUp() = default;

		SDL_FRect GetCollider() const override;

		bool Consume() override;

		void Update(float a_fDeltaTime);
		void Render(SDL_Renderer& a_Renderer) const;
	private:
		float m_fPowerUpWidth = 0;
		float m_fPowerUpHeight = 0;
		SDL_Texture* m_pPowerUpTexture = nullptr; // Not owned.

		bool m_bCanConsume = false;
		float m_fConsumeTimer = 0.0f;
		float m_fConsumeTime = 1.0f;

		EPowerUpType m_eEPowerUpType;
	};
}