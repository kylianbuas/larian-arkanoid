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
namespace arkanoid::game
{
	class Laser : public IGameObject
	{
	public:
		Laser(float a_fPosX, float a_fPosY);
		virtual ~Laser() = default;

		SDL_FRect GetCollider() const;

		void Update(float a_fDeltaTime);
		void Render(SDL_Renderer& a_Renderer) const;
	private:
		float m_fLaserWidth = 0;
		float m_fLaserHeight = 0;
		SDL_Texture* m_pLaserTexture = nullptr; // Not owned.
	};
}