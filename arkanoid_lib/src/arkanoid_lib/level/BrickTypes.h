#pragma once

#include <cstdint>

namespace arkanoid::level
{
	enum class EBrickType : uint8_t
	{
		EMPTY = 0,
		RED,
		BLUE,
		GREEN,
		YELLOW,
		ORANGE,
		PURPLE,
		CYAN,
		WHITE,
		RAINBOW,
		QUESTIONMARK,
		BRICK,
		GROUND,
		BLACK,
		GRAY,
		MAX = GRAY,
	};
}