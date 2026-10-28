#pragma once

#include <cstdint>
#include <imgui/imgui.h>
#include <vector>

struct SDL_Texture;

namespace arkanoid::level
{
	enum class EBrickType : uint8_t;
}
namespace arkanoid::imgui
{
	class LevelEditorWindow
	{
	public:
		void Initialize();
		void Render();
	private:
		void RenderTopToolbar();
		void RenderGrid();
		void RenderBottomToolbar();

		void ClearLevel();
		bool Save();
		bool Load();

		SDL_Texture* m_pBrickTexture = nullptr; // Not owned.
		level::EBrickType m_eSelectedBrickType;
		ImVec2 m_vBrickSize;
		std::vector<level::EBrickType> m_aBricks;
	};
}