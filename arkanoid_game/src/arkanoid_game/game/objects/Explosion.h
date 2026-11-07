#pragma once

#include <cstdint>

#include "arkanoid_game/game/IGameObject.h"

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
	class Explosion : public IGameObject
	{
	public:
		Explosion(float a_fPosX, float a_fPosY);
		virtual ~Explosion() = default;

		void Update(float a_fDeltaTime);
		void Render(SDL_Renderer& a_Renderer) const;
	private:
		SDL_Texture* m_pExplosionTexture = nullptr; // Not owned.
		int m_iExplosionIndex = 0;
		float m_fExplosionTimer = 0;
		float m_fExplosionTime = 0.1f;
		float m_iMaxExplosionFrames = 3;
		float m_fExplosionWidth = 0;
		float m_fExplosionHeight = 0;
	};
}