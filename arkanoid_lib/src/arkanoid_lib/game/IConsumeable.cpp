#include "./IConsumeable.h"

#include "arkanoid_lib/game/Game.h"

namespace arkanoid::game
{
	IConsumeable::IConsumeable()
	{
		GetGame().GetWorld().AddConsumeable(*this);
	}

	IConsumeable::~IConsumeable()
	{
		game::GetGame().GetWorld().RemoveConsumeable(*this);
	}
}