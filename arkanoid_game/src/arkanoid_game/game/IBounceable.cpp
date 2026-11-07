#include "./IBounceable.h"

#include "arkanoid_game/game/Game.h"

namespace arkanoid::game
{
	IBounceable::IBounceable()
	{
		GetGame().GetWorld().AddBounceable(*this);
	}

	IBounceable::~IBounceable()
	{
		game::GetGame().GetWorld().RemoveBounceable(*this);
	}
}