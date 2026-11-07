#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#include "arkanoid_lib/level/BrickTypes.h"
#include "arkanoid_lib/resources/ResourceSystem.h"
#include "arkanoid_lib/text/TextSystem.h"

namespace arkanoid::editor
{
	class Editor;
	extern Editor& GetEditor();

	class Editor
	{
	public:
		Editor(SDL_Window& a_Window, SDL_Renderer& a_Renderer);

		void LoadTextures();
		void Initialize();
		void Destroy();

		bool ProcessEvent(const SDL_Event& a_Event);
		void Render();

		void SetLevelPath(const std::filesystem::path& a_Path);

		SDL_Window& GetWindow();
		SDL_Renderer& GetRenderer();
		resources::ResourceSystem& GetResourceSystem();

		float GetContentWidth() const;
		float GetContentHeight() const;
	private:
		void ComputeLayout();

		void MoveCursor(int a_iDeltaX, int a_iDeltaY);
		void MovePaletteCursor(int a_iDeltaX, int a_iDeltaY);
		void PlaceSelectedBrick();
		void SelectBrick(int a_iIndex);
		void ClearLevel();

		void Save();
		void SaveAs();
		void Load();

		bool SaveToPath(const std::filesystem::path& a_Path);
		bool LoadFromPath(const std::filesystem::path& a_Path);

		void DrawGrid();
		void DrawPalette();
		void DrawHud();

		void DrawBrickTile(level::EBrickType a_eType, const SDL_FRect& a_Rect);
		void DrawSelectionOutline(const SDL_FRect& a_Rect);

		level::EBrickType GetBrickAt(size_t a_iX, size_t a_iY) const;
		void SetBrickAt(size_t a_iX, size_t a_iY, level::EBrickType a_eType);
	private:
		SDL_Window& m_Window;
		SDL_Renderer& m_Renderer;

		resources::ResourceSystem m_ResourceSystem;
		text::TextSystem m_TextSystem;

		std::vector<level::EBrickType> m_aBricks;
		std::filesystem::path m_LevelPath;

		SDL_Texture* m_pBrickTexture = nullptr; // Not owned.

		float m_fBrickWidth = 0.0f;
		float m_fBrickHeight = 0.0f;

		float m_fGridX = 0.0f;
		float m_fGridY = 0.0f;

		float m_fPaletteX = 0.0f;
		float m_fPaletteY = 0.0f;
		float m_fPaletteTileWidth = 0.0f;
		float m_fPaletteTileHeight = 0.0f;
		float m_fPaletteGap = 0.0f;

		float m_fControlsY = 0.0f;
		float m_fControlLineSpacing = 0.0f;

		float m_fContentWidth = 0.0f;
		float m_fContentHeight = 0.0f;

		int m_iCursorX = 0;
		int m_iCursorY = 0;
		int m_iSelectedBrick = static_cast<int>(level::EBrickType::RED);
	};
}
