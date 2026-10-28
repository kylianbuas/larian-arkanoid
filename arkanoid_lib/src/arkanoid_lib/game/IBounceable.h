#pragma once

#include <SDL3/SDL_rect.h>

namespace arkanoid::game
{
	class IBounceable
	{
	public:
		virtual ~IBounceable() = default;

		virtual SDL_FRect GetCollider() const = 0;

		virtual void OnHit()
		{};

		// Return true if the bounce needs to be overridden.
		virtual bool Bounce(float a_fOverlapX, float a_fOverlapY, float& a_fDirectionX, float& a_fDirectionY)
		{
			return false;
		};
	};
}