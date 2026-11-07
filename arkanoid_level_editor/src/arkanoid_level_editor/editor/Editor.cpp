#include "./Editor.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "arkanoid_lib/level/LevelData.h"
#include "arkanoid_level_editor/windows/WinFile.h"
#include "arkanoid_lib/game/Brick.h"

namespace arkanoid::editor
{
	namespace
	{
		constexpr int NUM_PALETTE_COLUMNS = 3;

		constexpr int NUM_BRICK_TYPES = static_cast<int>(level::EBrickType::MAX) + 1;

		constexpr int NUM_CONTROL_LINES = 7;

		const std::vector<COMDLG_FILTERSPEC> kLevelFilters =
		{
			{ L"Level files (*.lvl)", L"*.lvl" },
			{ L"All files (*.*)", L"*.*" },
		};

		constexpr const char* CONTROLS[NUM_CONTROL_LINES] =
		{
			"WASD: MOVE GRID",
			"ARROWS: SELECT BRICK",
			"ENTER: PLACE",
			"CTRL+S: SAVE",
			"CTRL+SHIFT+S: SAVE AS",
			"CTRL+O: OPEN",
			"N: CLEAR",
		};
	}

	Editor::Editor(SDL_Window& a_Window, SDL_Renderer& a_Renderer) :
		m_Window(a_Window),
		m_Renderer(a_Renderer),
		m_ResourceSystem(a_Renderer),
		m_TextSystem(a_Renderer)
	{
		m_aBricks.assign(level::LEVEL_WIDTH * level::LEVEL_HEIGHT, level::EBrickType::EMPTY);

		SDL_Log("Working directory: %s", SDL_GetCurrentDirectory());
	}

	void Editor::LoadTextures()
	{
		m_ResourceSystem.LoadTexture(resources::ETextureType::DUMMY, "tex_dummy.png"); // Used for measuring the bricks.

		SDL_Texture* pDummyTexture = m_ResourceSystem.GetTexture(resources::ETextureType::DUMMY);
		if (!pDummyTexture || !SDL_GetTextureSize(pDummyTexture, &m_fBrickWidth, &m_fBrickHeight))
		{
			m_fBrickWidth = game::DUMMY_DEFAULT_WIDTH;
			m_fBrickHeight = game::DUMMY_DEFAULT_HEIGHT;
		}

		m_ResourceSystem.LoadTexture(resources::ETextureType::BRICK, "tex_bricks.png");
		m_pBrickTexture = m_ResourceSystem.GetTexture(resources::ETextureType::BRICK);

		ComputeLayout();
	}

	void Editor::ComputeLayout()
	{
		float tileScale = 3.0f;
		const float tileWidth = m_fBrickWidth * tileScale;
		const float tileHeight = m_fBrickHeight * tileScale;
		const float gap = m_fBrickHeight * 0.5f;

		const float gridWidth = static_cast<float>(level::LEVEL_WIDTH) * m_fBrickWidth;
		const float gridHeight = static_cast<float>(level::LEVEL_HEIGHT) * m_fBrickHeight;

		const size_t paletteRows = (NUM_BRICK_TYPES + NUM_PALETTE_COLUMNS - 1) / NUM_PALETTE_COLUMNS;
		const float paletteWidth = static_cast<float>(NUM_PALETTE_COLUMNS) * tileWidth +
			static_cast<float>(NUM_PALETTE_COLUMNS - 1) * gap;
		const float paletteHeight = static_cast<float>(paletteRows) * tileHeight +
			static_cast<float>(paletteRows - 1) * gap;

		m_fGridX = m_fBrickWidth;
		m_fGridY = m_fBrickHeight;

		m_fPaletteTileWidth = tileWidth;
		m_fPaletteTileHeight = tileHeight;
		m_fPaletteGap = gap;
		m_fPaletteX = m_fGridX + gridWidth + (m_fBrickWidth * 1.25f);
		m_fPaletteY = m_fGridY + (m_fBrickHeight * 0.5f);

		m_fControlLineSpacing = m_fBrickHeight * 2.0f;
		m_fControlsY = (std::max)(m_fGridY + gridHeight, m_fPaletteY + paletteHeight) +
			(m_fBrickHeight * 0.75f);

		m_fContentWidth = m_fPaletteX + paletteWidth + m_fBrickWidth;
		m_fContentHeight = m_fControlsY + (static_cast<float>(NUM_CONTROL_LINES) * m_fControlLineSpacing) +
			(m_fBrickHeight * 0.25f);
	}

	float Editor::GetContentWidth() const
	{
		return m_fContentWidth;
	}

	float Editor::GetContentHeight() const
	{
		return m_fContentHeight;
	}

	void Editor::Initialize()
	{
		if (!m_LevelPath.empty() && std::filesystem::exists(m_LevelPath))
		{
			LoadFromPath(m_LevelPath);
		}
	}

	void Editor::Destroy()
	{
		m_TextSystem.Destroy();
		m_ResourceSystem.Destroy();
	}

	void Editor::SetLevelPath(const std::filesystem::path& a_Path)
	{
		m_LevelPath = a_Path;
	}

	SDL_Window& Editor::GetWindow()
	{
		return m_Window;
	}

	SDL_Renderer& Editor::GetRenderer()
	{
		return m_Renderer;
	}

	resources::ResourceSystem& Editor::GetResourceSystem()
	{
		return m_ResourceSystem;
	}

	bool Editor::ProcessEvent(const SDL_Event& a_Event)
	{
		if (a_Event.type == SDL_EVENT_QUIT)
		{
			return false;
		}

		if (a_Event.type != SDL_EVENT_KEY_DOWN)
		{
			return true;
		}

		const SDL_Keycode key = a_Event.key.key;
		const SDL_Keymod mods = a_Event.key.mod;

		if ((mods & SDL_KMOD_CTRL) != 0)
		{
			switch (key)
			{
				case SDLK_S:
				{
					if (!a_Event.key.repeat)
					{
						if ((mods & SDL_KMOD_SHIFT) != 0)
						{
							SaveAs();
						}
						else
						{
							Save();
						}
					}
					return true;
				}
				case SDLK_O:
				{
					if (!a_Event.key.repeat)
					{
						Load();
					}
					return true;
				}
				default:
				{
					break;
				}
			}
		}

		switch (key)
		{
			case SDLK_UP:
			{
				MovePaletteCursor(0, -1);
				return true;
			}
			case SDLK_DOWN:
			{
				MovePaletteCursor(0, 1);
				return true;
			}
			case SDLK_LEFT:
			{
				MovePaletteCursor(-1, 0);
				return true;
			}
			case SDLK_RIGHT:
			{
				MovePaletteCursor(1, 0);
				return true;
			}
			case SDLK_W:
			{
				MoveCursor(0, -1);
				return true;
			}
			case SDLK_A:
			{
				MoveCursor(-1, 0);
				return true;
			}
			case SDLK_S:
			{
				MoveCursor(0, 1);
				return true;
			}
			case SDLK_D:
			{
				MoveCursor(1, 0);
				return true;
			}
			default:
			{
				break;
			}
		}

		if (a_Event.key.repeat)
		{
			return true;
		}

		switch (key)
		{
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_SPACE:
			{
				PlaceSelectedBrick();
				break;
			}
			case SDLK_N:
			{
				ClearLevel();
				break;
			}
		}

		return true;
	}

	void Editor::Render()
	{
		SDL_SetRenderDrawColor(&m_Renderer, 15, 20, 35, 255);
		SDL_RenderClear(&m_Renderer);

		DrawGrid();
		DrawPalette();
		DrawHud();
	}

	void Editor::MoveCursor(int a_iDeltaX, int a_iDeltaY)
	{
		m_iCursorX = std::clamp(m_iCursorX + a_iDeltaX, 0, static_cast<int>(level::LEVEL_WIDTH) - 1);
		m_iCursorY = std::clamp(m_iCursorY + a_iDeltaY, 0, static_cast<int>(level::LEVEL_HEIGHT) - 1);
	}

	void Editor::MovePaletteCursor(int a_iDeltaX, int a_iDeltaY)
	{
		int column = m_iSelectedBrick % NUM_PALETTE_COLUMNS;
		int row = m_iSelectedBrick / NUM_PALETTE_COLUMNS;
		const int rowCount = (NUM_BRICK_TYPES + NUM_PALETTE_COLUMNS - 1) / NUM_PALETTE_COLUMNS;

		column = std::clamp(column + a_iDeltaX, 0, NUM_PALETTE_COLUMNS - 1);
		row = std::clamp(row + a_iDeltaY, 0, rowCount - 1);

		const int index = (row * NUM_PALETTE_COLUMNS) + column;
		if (index < NUM_BRICK_TYPES)
		{
			m_iSelectedBrick = index;
		}
	}

	void Editor::PlaceSelectedBrick()
	{
		const level::EBrickType selected = static_cast<level::EBrickType>(m_iSelectedBrick);
		SetBrickAt(m_iCursorX, m_iCursorY, selected);
	}

	void Editor::SelectBrick(int a_iIndex)
	{
		if (a_iIndex < 0 || a_iIndex >= NUM_BRICK_TYPES)
		{
			return;
		}

		m_iSelectedBrick = a_iIndex;
	}

	void Editor::ClearLevel()
	{
		m_aBricks.assign(level::LEVEL_WIDTH * level::LEVEL_HEIGHT, level::EBrickType::EMPTY);
	}

	void Editor::Save()
	{
		if (m_LevelPath.empty())
		{
			SaveAs();
			return;
		}

		SaveToPath(m_LevelPath);
	}

	void Editor::SaveAs()
	{
		std::filesystem::path path = m_LevelPath;
		if (!windows::SaveFile(path, kLevelFilters))
		{
			return;
		}

		SaveToPath(path);
	}

	void Editor::Load()
	{
		std::filesystem::path path = m_LevelPath;
		if (!windows::PickFile(path, kLevelFilters))
		{
			return;
		}

		LoadFromPath(path);
	}

	bool Editor::SaveToPath(const std::filesystem::path& a_Path)
	{
		std::filesystem::path path = a_Path;
		if (path.extension().empty())
		{
			path += ".lvl";
		}

		if (path.has_parent_path())
		{
			std::filesystem::create_directories(path.parent_path());
		}

		std::ofstream file(path, std::ios::binary | std::ios::trunc);
		if (!file)
		{
			return false;
		}

		size_t count = m_aBricks.size();
		while (count > 0 && m_aBricks[count - 1] == level::EBrickType::EMPTY)
		{
			count--;
		}

		for (size_t i = 0; i < count; i++)
		{
			const uint8_t value = static_cast<uint8_t>(m_aBricks[i]);
			file.put(static_cast<char>(value));
		}

		if (!file)
		{
			return false;
		}

		m_LevelPath = path;
		return true;
	}

	bool Editor::LoadFromPath(const std::filesystem::path& a_Path)
	{
		std::ifstream file(a_Path, std::ios::binary | std::ios::ate);
		if (!file)
		{
			return false;
		}

		const std::streamoff fileSize = file.tellg();
		const size_t maxSize = level::LEVEL_WIDTH * level::LEVEL_HEIGHT;

		if (fileSize < 0 || static_cast<size_t>(fileSize) > maxSize)
		{
			return false;
		}

		std::vector<uint8_t> data(static_cast<size_t>(fileSize));
		file.seekg(0, std::ios::beg);

		if (!data.empty() && !file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size())))
		{
			return false;
		}

		const uint8_t maxType = static_cast<uint8_t>(level::EBrickType::MAX);
		for (uint8_t value : data)
		{
			if (value > maxType)
			{
				return false;
			}
		}

		m_aBricks.assign(maxSize, level::EBrickType::EMPTY);
		for (size_t i = 0; i < data.size(); i++)
		{
			m_aBricks[i] = static_cast<level::EBrickType>(data[i]);
		}

		m_LevelPath = a_Path;
		return true;
	}

	void Editor::DrawGrid()
	{
		const float cellWidth = m_fBrickWidth;
		const float cellHeight = m_fBrickHeight;
		const float gridWidth = static_cast<float>(level::LEVEL_WIDTH) * cellWidth;
		const float gridHeight = static_cast<float>(level::LEVEL_HEIGHT) * cellHeight;

		SDL_SetRenderDrawColor(&m_Renderer, 22, 28, 44, 255);
		const float padding = m_fBrickHeight * 0.5f;
		const SDL_FRect gridBackground = {
			m_fGridX - padding,
			m_fGridY - padding,
			gridWidth + (padding * 2.0f),
			gridHeight + (padding * 2.0f),
		};
		SDL_RenderFillRect(&m_Renderer, &gridBackground);

		for (size_t y = 0; y < level::LEVEL_HEIGHT; y++)
		{
			for (size_t x = 0; x < level::LEVEL_WIDTH; x++)
			{
				const SDL_FRect cell = {
					m_fGridX + (x * cellWidth),
					m_fGridY + (y * cellHeight),
					cellWidth,
					cellHeight,
				};

				const level::EBrickType type = GetBrickAt(x, y);
				if (type == level::EBrickType::EMPTY)
				{
					SDL_SetRenderDrawColor(&m_Renderer, 28, 35, 54, 255);
					SDL_RenderFillRect(&m_Renderer, &cell);
				}
				else
				{
					DrawBrickTile(type, cell);
				}
			}
		}

		SDL_SetRenderDrawColor(&m_Renderer, 42, 50, 72, 255);
		for (size_t x = 0; x <= level::LEVEL_WIDTH; x++)
		{
			const SDL_FRect line = { m_fGridX + (x * cellWidth), m_fGridY, 1.0f, gridHeight };
			SDL_RenderFillRect(&m_Renderer, &line);
		}
		for (size_t y = 0; y <= level::LEVEL_HEIGHT; y++)
		{
			const SDL_FRect line = { m_fGridX, m_fGridY + (y * cellHeight), gridWidth, 1.0f };
			SDL_RenderFillRect(&m_Renderer, &line);
		}

		const SDL_FRect cursor = {
			m_fGridX + (m_iCursorX * cellWidth) - 1.0f,
			m_fGridY + (m_iCursorY * cellHeight) - 1.0f,
			cellWidth + 2.0f,
			cellHeight + 2.0f,
		};

		SDL_SetRenderDrawColor(&m_Renderer, 255, 215, 70, 255);
		SDL_RenderRect(&m_Renderer, &cursor);
	}

	void Editor::DrawPalette()
	{
		m_TextSystem.DisplayText("BRICK PALETTE", static_cast<size_t>(m_fPaletteX), static_cast<size_t>(m_fPaletteY - (m_fBrickHeight * 2.5f)));

		for (size_t i = 0; i < NUM_BRICK_TYPES; i++)
		{
			const size_t column = i % NUM_PALETTE_COLUMNS;
			const size_t row = i / NUM_PALETTE_COLUMNS;

			const SDL_FRect tile = {
				m_fPaletteX + (column * (m_fPaletteTileWidth + m_fPaletteGap)),
				m_fPaletteY + (row * (m_fPaletteTileHeight + m_fPaletteGap)),
				m_fPaletteTileWidth,
				m_fPaletteTileHeight,
			};

			DrawBrickTile(static_cast<level::EBrickType>(i), tile);

			if (i == m_iSelectedBrick)
			{
				DrawSelectionOutline(tile);
			}
		}
	}

	void Editor::DrawHud()
	{
		for (size_t i = 0; i < NUM_CONTROL_LINES; i++)
		{
			const size_t y = static_cast<size_t>(m_fControlsY + (i * m_fControlLineSpacing));
			m_TextSystem.DisplayText(CONTROLS[i], static_cast<size_t>(m_fGridX), y);
		}
	}

	void Editor::DrawBrickTile(level::EBrickType a_eType, const SDL_FRect& a_Rect)
	{
		if (a_eType == level::EBrickType::EMPTY)
		{
			SDL_SetRenderDrawColor(&m_Renderer, 34, 41, 62, 255);
			SDL_RenderFillRect(&m_Renderer, &a_Rect);

			SDL_SetRenderDrawColor(&m_Renderer, 110, 120, 145, 255);
		}
		else
		{
			if (m_pBrickTexture)
			{
				const size_t typeIndex = static_cast<int>(a_eType) - 1;
				const SDL_FRect source = {
					typeIndex * m_fBrickWidth,
					0.0f,
					m_fBrickWidth,
					m_fBrickHeight,
				};

				SDL_RenderTexture(&m_Renderer, m_pBrickTexture, &source, &a_Rect);
			}
			else
			{
				Uint8 r = 0;
				Uint8 g = 0;
				Uint8 b = 0;
				level::GetBrickColor(a_eType, r, g, b);

				SDL_SetRenderDrawColor(&m_Renderer, r, g, b, 255);
				SDL_RenderFillRect(&m_Renderer, &a_Rect);
			}
		}
	}

	void Editor::DrawSelectionOutline(const SDL_FRect& a_Rect)
	{
		SDL_SetRenderDrawColor(&m_Renderer, 255, 215, 70, 255);

		for (size_t i = 0; i < 2; i++)
		{
			const SDL_FRect outline = {
				a_Rect.x - static_cast<float>(i + 1),
				a_Rect.y - static_cast<float>(i + 1),
				a_Rect.w + static_cast<float>((i + 1) * 2),
				a_Rect.h + static_cast<float>((i + 1) * 2),
			};
			SDL_RenderRect(&m_Renderer, &outline);
		}
	}

	level::EBrickType Editor::GetBrickAt(size_t a_iX, size_t a_iY) const
	{
		if (a_iX < 0 || a_iX >= level::LEVEL_WIDTH || a_iY < 0 || a_iY >= level::LEVEL_HEIGHT)
		{
			return level::EBrickType::EMPTY;
		}

		return m_aBricks[(a_iY * level::LEVEL_WIDTH) + a_iX];
	}

	void Editor::SetBrickAt(size_t a_iX, size_t a_iY, level::EBrickType a_eType)
	{
		if (a_iX < 0 || a_iX >= level::LEVEL_WIDTH || a_iY < 0 || a_iY >= level::LEVEL_HEIGHT)
		{
			return;
		}

		m_aBricks[(a_iY * level::LEVEL_WIDTH) + a_iX] = a_eType;
	}
}
