#pragma once

#include <SDL3/SDL_rect.h>

namespace arkanoid::game
{
	class IConsumeable
	{
	public:
		IConsumeable();
		virtual ~IConsumeable();

		virtual SDL_FRect GetCollider() const = 0;

		virtual bool Consume()
		{
			return false;
		};
	};
}