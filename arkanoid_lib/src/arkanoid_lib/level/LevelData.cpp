#include "./LevelData.h"

#include "arkanoid_lib/level/BrickTypes.h"

namespace arkanoid::level
{
	bool LevelData::Load(const std::vector<uint8_t>& a_aLevelData)
	{
		if (a_aLevelData.empty())
		{
			return false;
		}

		m_aBricks.clear();
		m_aBricks.reserve(a_aLevelData.size());
		for (uint8_t value : a_aLevelData)
		{
			m_aBricks.push_back(static_cast<EBrickType>(value));
		}

		return true;
	}

	const std::vector<EBrickType>& LevelData::GetBricks() const
	{
		return m_aBricks;
	}

	EBrickType LevelData::GetBrickAt(size_t x, size_t y) const
	{
		if (x >= LEVEL_WIDTH || y >= LEVEL_HEIGHT)
		{
			return EBrickType::EMPTY;
		}

		const size_t index = x + (y * x);
		if (index >= m_aBricks.size())
		{
			return EBrickType::EMPTY;
		}

		return m_aBricks[index];
	}
}