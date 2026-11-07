#pragma once

#include <cstdint>

namespace arkanoid::game
{
	enum class EPowerUpType : uint8_t
	{
		ONE_UP = 0,
		PLACEHOLDER,
		PLACEHOLDER_2,
		PLACEHOLDER_3,
		PLACEHOLDER_4,
		PLACEHOLDER_5,
		PLACEHOLDER_6,
		MAX,
	};

	inline void GetPowerUpColor(EPowerUpType a_eType, uint8_t& a_iR, uint8_t& a_iG, uint8_t& a_iB)
	{
		a_iR = 0;
		a_iG = 0;
		a_iB = 0;

		switch (a_eType)
		{
			case EPowerUpType::ONE_UP:
			{
				a_iR = 0;
				a_iG = 255;
				a_iB = 122;
				break;
			}
			default:
			{
				break;
			}
		}
	}
}