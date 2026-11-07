#pragma once

#include <cstdint>

namespace arkanoid::level
{
	// These are used for measuring the game.
	// Logic, size of textures and their positioning inside the game world are dependent on this.
	constexpr float DUMMY_DEFAULT_WIDTH = 16;
	constexpr float DUMMY_DEFAULT_HEIGHT = 8;

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

	inline void GetBrickColor(EBrickType a_eType, uint8_t& a_iR, uint8_t& a_iG, uint8_t& a_iB)
	{
		a_iR = 0;
		a_iG = 0;
		a_iB = 0;

		switch (a_eType)
		{
			case EBrickType::RED:
			{
				a_iR = 255; 
				a_iG = 0;   
				a_iB = 0;   
				break;
			}
			case EBrickType::BLUE:
			{
				a_iR = 0;   
				a_iG = 0;   
				a_iB = 255; 
				break;
			}
			case EBrickType::GREEN:
			{
				a_iR = 0;   
				a_iG = 255; 
				a_iB = 0;   
				break;
			}
			case EBrickType::YELLOW:
			{
				a_iR = 255; 
				a_iG = 216; 
				a_iB = 0;   
				break;
			}
			case EBrickType::ORANGE:
			{
				a_iR = 255; 
				a_iG = 106; 
				a_iB = 0;   
				break;
			}
			case EBrickType::PURPLE:
			{
				a_iR = 178; 
				a_iG = 0;  
				a_iB = 255; 
				break;
			}
			case EBrickType::CYAN:
			{
				a_iR = 0;   
				a_iG = 255; 
				a_iB = 255;
				break;
			}
			case EBrickType::WHITE:
			{
				a_iR = 255; 
				a_iG = 255; 
				a_iB = 255; break;
			}
			case EBrickType::RAINBOW: 
			{
				a_iR = 50;  
				a_iG = 50;  
				a_iB = 255; 
				break;
			}
			case EBrickType::QUESTIONMARK: {
				a_iR = 255; 
				a_iG = 216; 
				a_iB = 0;   
				break;
			}
			case EBrickType::BRICK: 
			{
				a_iR = 151; 
				a_iG = 94;  
				a_iB = 45;  
				break;
			}
			case EBrickType::GROUND: 
			{
				a_iR = 185; 
				a_iG = 99;  
				
				a_iB = 55;  
				break;
			}
			case EBrickType::BLACK: 
			{
				a_iR = 0;   
				a_iG = 0;   
				a_iB = 0;   
				break;
			}
			case EBrickType::GRAY: 
			{
				a_iR = 94;  
				a_iG = 94;  
				a_iB = 94;  
				break;
			}
			default: { break; }
		}
	}
}