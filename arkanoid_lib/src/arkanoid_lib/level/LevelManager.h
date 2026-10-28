#pragma once

#include <vector>
#include <string>

namespace arkanoid::resources
{
	class ResourceSystem;
}
namespace arkanoid::level
{
	class Level;

	class LevelManager
	{
	public:
		LevelManager(resources::ResourceSystem& a_ResourceSystem);
		void SetLevels(const std::vector<std::string>& a_aLevelNames);

		bool LoadLevel(size_t a_iLevelIndex, Level& a_Level);
		bool LoadNextLevel(Level& a_Level);

		size_t GetCurrentLevelIndex() const;
		size_t GetLevelCount() const;
	private:
		resources::ResourceSystem& m_ResourceSystem;

		size_t m_iCurrentLevel = 0;
		std::vector<std::string> m_aLevelNames;
	};
}