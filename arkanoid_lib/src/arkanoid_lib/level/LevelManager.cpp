#include "./LevelManager.h"

#include "arkanoid_lib/level/Level.h"
#include "arkanoid_lib/resources/ResourceSystem.h"

namespace arkanoid::level
{
	LevelManager::LevelManager(resources::ResourceSystem& a_ResourceSystem) :
		m_ResourceSystem(a_ResourceSystem)
	{}

	void LevelManager::SetLevels(const std::vector<std::string>& a_aLevelNames)
	{
		m_aLevelNames = a_aLevelNames;
	}

	bool LevelManager::LoadLevel(size_t a_iLevelIndex, Level& a_Level)
	{
		if (a_iLevelIndex >= m_aLevelNames.size()) // This is a public method, so still do this.
		{
			return false;
		}

		m_iCurrentLevel = a_iLevelIndex;
		level::LevelData levelData = m_ResourceSystem.LoadLevel(m_aLevelNames[a_iLevelIndex]);
		return a_Level.Initialize(levelData);
	}

	bool LevelManager::LoadNextLevel(Level& a_Level)
	{
		size_t level = (m_iCurrentLevel + 1) % m_aLevelNames.size();
		return LoadLevel(level, a_Level);
	}

	size_t LevelManager::GetCurrentLevelIndex() const
	{
		return m_iCurrentLevel;
	}

	size_t LevelManager::GetLevelCount() const
	{
		return m_aLevelNames.size();
	}
}