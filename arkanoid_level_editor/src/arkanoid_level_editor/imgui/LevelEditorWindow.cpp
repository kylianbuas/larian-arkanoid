#include "./LevelEditorWindow.h"

#include <imgui.h>
#include <SDL.h>

#include "arkanoid_lib/level/BrickTypes.h"
#include "arkanoid_lib/level/LevelData.h"

#include "arkanoid_level_editor/editor/Editor.h"
#include <arkanoid_level_editor/windows/WinFile.h>

namespace arkanoid::imgui
{
	void LevelEditorWindow::Initialize()
	{
		m_pBrickTexture = editor::GetEditor().GetResourceSystem().GetTexture(resources::ETextureType::BRICK);

		float textureWidth = 0.0f;
		float textureHeight = 0.0f;
		SDL_GetTextureSize(m_pBrickTexture, &textureWidth, &textureHeight);

		const int numBrickTypes = static_cast<int>(level::EBrickType::MAX);

		m_vBrickSize = { textureWidth / numBrickTypes, textureHeight };

		m_aBricks.reserve(level::LEVEL_WIDTH * level::LEVEL_HEIGHT);
		ClearLevel();

		m_eSelectedBrickType = level::EBrickType::EMPTY;
	}

	void LevelEditorWindow::Render()
	{
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

		ImGui::Begin("Level Editor", nullptr,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar);

		RenderTopToolbar();
		RenderGrid();
		RenderBottomToolbar();

		ImGui::End();
	}

	void LevelEditorWindow::RenderTopToolbar()
	{
		const float toolbarHeight = ImGui::GetFrameHeight() + ImGui::GetStyle().WindowPadding.y * 2.0f;
		if (ImGui::BeginChild("TopToolbar", ImVec2(0.0f, toolbarHeight), ImGuiChildFlags_Borders))
		{
			if (ImGui::Button("Open Level"))
			{
				Load();
			}

			ImGui::SameLine();

			if (ImGui::Button("Save Level"))
			{
				Save();
			}

			ImGui::SameLine();

			if (ImGui::Button("Clear Level"))
			{
				ClearLevel();
			}
		}

		ImGui::EndChild();
	}

	void LevelEditorWindow::RenderGrid()
	{
		const float availableWidth = ImGui::GetContentRegionAvail().x;
		const float totalSpacing = (level::LEVEL_WIDTH - 1) * ImGui::GetStyle().ItemSpacing.x;
		const float totalPadding = level::LEVEL_WIDTH * ImGui::GetStyle().FramePadding.x * 2.0f;
		const float scale = (availableWidth - totalSpacing - totalPadding) / (level::LEVEL_WIDTH * m_vBrickSize.x);

		const float gridHeight =
			(m_vBrickSize.y * scale * level::LEVEL_HEIGHT) +
			((level::LEVEL_HEIGHT - 1) * ImGui::GetStyle().ItemSpacing.y) +
			(level::LEVEL_HEIGHT * ImGui::GetStyle().FramePadding.y * 2.0f);

		if (ImGui::BeginChild("Grid", ImVec2(0.0f, gridHeight), 0))
		{
			const float availableWidth = ImGui::GetContentRegionAvail().x;
			const float totalSpacing = (level::LEVEL_WIDTH - 1) * ImGui::GetStyle().ItemSpacing.x;
			const float totalPadding = level::LEVEL_WIDTH * ImGui::GetStyle().FramePadding.x * 2.0f;

			const float scale = (availableWidth - totalSpacing - totalPadding) / (level::LEVEL_WIDTH * m_vBrickSize.x);

			const ImVec2 brickSize(
				m_vBrickSize.x * scale,
				m_vBrickSize.y * scale);

			for (int y = 0; y < level::LEVEL_HEIGHT; y++)
			{
				for (int x = 0; x < level::LEVEL_WIDTH; x++)
				{
					const int index = y * level::LEVEL_WIDTH + x;
					const level::EBrickType brickType = m_aBricks[index];

					ImGui::PushID(index);

					if (x > 0)
					{
						ImGui::SameLine();
					}

					if (brickType == level::EBrickType::EMPTY)
					{
						if (ImGui::ImageButton("##Brick", nullptr, brickSize))
						{
							m_aBricks[index] = m_eSelectedBrickType;
						}
					}
					else
					{
						const int numBrickTypes = static_cast<int>(level::EBrickType::MAX);
						size_t current = static_cast<size_t>(m_aBricks[index]) - 1;

						const ImVec2 uv0(
							static_cast<float>(current) / numBrickTypes,
							0.0f);

						const ImVec2 uv1(
							static_cast<float>(current + 1) / numBrickTypes,
							1.0f);

						if (ImGui::ImageButton("##Brick", ImTextureRef(m_pBrickTexture), brickSize, uv0, uv1))
						{
							m_aBricks[index] = m_eSelectedBrickType;
						}
					}

					ImGui::PopID();
				}
			}
		}

		ImGui::EndChild();
	}

	void LevelEditorWindow::RenderBottomToolbar()
	{
		if (ImGui::BeginChild("BottomToolbar", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders))
		{
			const int numBrickTypes = static_cast<int>(level::EBrickType::MAX);
			const int buttonCount = numBrickTypes + 1;
			const float availableWidth = ImGui::GetContentRegionAvail().x;
			const float totalSpacing = (buttonCount - 1) * ImGui::GetStyle().ItemSpacing.x;
			const float totalPadding = buttonCount * ImGui::GetStyle().FramePadding.x * 2.0f;

			const float scale = (availableWidth - totalSpacing - totalPadding) / (buttonCount * m_vBrickSize.x);

			const float brickWidth = m_vBrickSize.x * scale;
			const float brickHeight = m_vBrickSize.y * scale;

			const ImVec2 toolbarTileSize = {
				brickWidth,
				brickHeight,
			};

			for (size_t i = 0; i < numBrickTypes + 1; i++)
			{
				ImGui::PushID(i);

				if (i > 0)
				{
					ImGui::SameLine();
				}

				if (i == 0)
				{
					if (ImGui::ImageButton("##Tile", nullptr, toolbarTileSize))
					{
						m_eSelectedBrickType = static_cast<level::EBrickType>(i);
					}
				}
				else
				{
					const ImVec2 uv0(
						static_cast<float>(i - 1) / numBrickTypes,
						0.0f);

					const ImVec2 uv1(
						static_cast<float>(i) / numBrickTypes,
						1.0f);

					if (ImGui::ImageButton("##Tile", ImTextureRef(m_pBrickTexture), toolbarTileSize, uv0, uv1))
					{
						m_eSelectedBrickType = static_cast<level::EBrickType>(i);
					}
				}

				if (static_cast<level::EBrickType>(i) == m_eSelectedBrickType)
				{
					ImGui::GetWindowDrawList()->AddRect(
						ImGui::GetItemRectMin(),
						ImGui::GetItemRectMax(),
						IM_COL32(255, 200, 0, 255),
						0.0f,
						0,
						3.0f);
				}

				ImGui::PopID();
			}
		}

		ImGui::EndChild();
	}

	const std::vector<COMDLG_FILTERSPEC> filters =
	{
		{L"Level files (*.lvl)", L"*.lvl"}
	};

	void LevelEditorWindow::ClearLevel()
	{
		m_aBricks.assign(level::LEVEL_WIDTH * level::LEVEL_HEIGHT, level::EBrickType::EMPTY);
	}

	bool LevelEditorWindow::Save()
	{
		fs::path path;
		if (windows::SaveFile(path, filters))
		{
			FILE* file = nullptr;
			fopen_s(&file, path.string().c_str(), "wb");

			if (!file)
			{
				return false;
			}

			// To save file size, go in reverse and get the last non-empty brick.
			size_t size = 0;
			for (size_t i = m_aBricks.size(); i > 0; i--)
			{
				if (m_aBricks[i - 1] != level::EBrickType::EMPTY)
				{
					size = i;
					break;
				}
			}

			fwrite(m_aBricks.data(), size, sizeof(level::EBrickType), file);
			fclose(file);

			return true;
		}
		return false;
	}

	bool LevelEditorWindow::Load()
	{
		fs::path path;
		if (windows::PickFile(path, filters))
		{
			FILE* file = nullptr;
			fopen_s(&file, path.string().c_str(), "rb");

			if (!file)
			{
				return false;
			}

			// Only clear the old level when loading was successful.
			ClearLevel();

			fseek(file, 0, SEEK_END);
			long fileSize = ftell(file);
			rewind(file);

			if (fileSize < 0 || fileSize > level::LEVEL_WIDTH * level::LEVEL_HEIGHT)
			{
				fclose(file);
				return false;
			}

			fread(m_aBricks.data(), sizeof(uint8_t), fileSize, file);
			fclose(file);

			return true;
		}

		return false;
	}
}