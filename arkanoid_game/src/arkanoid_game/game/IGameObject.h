#pragma once

#include <cstdint>
#include <SDL3/SDL_rect.h>

struct SDL_Renderer;

namespace arkanoid::resources
{
	class ResourceSystem;
	class Sound;
}
namespace arkanoid::game
{
	class IGameObject
	{
	public:
		IGameObject(float a_fPosX, float a_fPosY);
		virtual ~IGameObject() = default;

		virtual void LoadTextures(resources::ResourceSystem& a_ResourceSystem);
		virtual void LoadSounds(resources::ResourceSystem& a_ResourceSystem);

		void Destroy();
		bool IsDestroyed() const;
		virtual void OnDestroyed()
		{}

		int GetPosX() const;
		int GetPosY() const;

		virtual void Update(float a_fDeltaTime) = 0;
		virtual void Render(SDL_Renderer& a_Renderer) const = 0;
	protected:
		bool m_bDestroyed = false;
		float m_fPosX = 0, m_fPosY = 0;
	};
}