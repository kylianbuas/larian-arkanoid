#pragma once

#include "arkanoid_lib/level/BrickTypes.h"
#include "arkanoid_lib/level/LevelData.h"
#include "arkanoid_game/game/PowerUpTypes.h"

namespace arkanoid::game
{
	constexpr float BALL_MOVEMENT_SPEED = 250.0f;
	constexpr float POWER_UP_FALL_SPEED = 75.0f;
	constexpr float LASER_SPEED = 300.f;
	constexpr int STARTING_LIVES = 3;
	constexpr float POWER_UP_SPAWN_CHANCE = 0.5f;
	constexpr float SHOOTING_POWER_UP_TIMER = 5.0f;
	constexpr float SHOOTING_POWER_UP_DELAY = 0.2f;
	constexpr EPowerUpType MAX_POWER_UP = EPowerUpType::SHOOTER;
	constexpr std::array<uint8_t, 48> FALLBACK_LEVEL = {
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::EMPTY),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::CYAN),
		static_cast<uint8_t>(level::EBrickType::RED),
		static_cast<uint8_t>(level::EBrickType::RED),
		static_cast<uint8_t>(level::EBrickType::RED),
		static_cast<uint8_t>(level::EBrickType::RED),
		static_cast<uint8_t>(level::EBrickType::YELLOW),
		static_cast<uint8_t>(level::EBrickType::YELLOW),
		static_cast<uint8_t>(level::EBrickType::YELLOW),
		static_cast<uint8_t>(level::EBrickType::YELLOW),
		static_cast<uint8_t>(level::EBrickType::YELLOW),
		static_cast<uint8_t>(level::EBrickType::YELLOW),
		static_cast<uint8_t>(level::EBrickType::YELLOW),
		static_cast<uint8_t>(level::EBrickType::YELLOW),
	};
}