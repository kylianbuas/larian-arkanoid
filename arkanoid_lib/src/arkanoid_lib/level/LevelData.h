#pragma once

#include <cstdint>
#include <vector>

namespace arkanoid::level
{
	enum class EBrickType : uint8_t;

	static constexpr uint16_t LEVEL_WIDTH = 12;
	static constexpr uint16_t LEVEL_HEIGHT = 18;
	static constexpr uint16_t PADDLE_POS_Y = 40;

	class LevelData
	{
	public:
		bool Load(const std::vector<uint8_t>& a_aLevelData);
		const std::vector<EBrickType>& GetBricks() const;
		EBrickType GetBrickAt(size_t x, size_t y) const;
	private:
		std::vector<EBrickType> m_aBricks;
	};
}