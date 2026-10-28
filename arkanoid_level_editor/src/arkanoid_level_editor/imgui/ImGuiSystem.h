#pragma once

#include <imgui/imgui.h>

#include "arkanoid_level_editor/imgui/LevelEditorWindow.h"

union SDL_Event;
struct SDL_Window;
struct SDL_Renderer;

namespace arkanoid::imgui
{
	class ImGuiSystem;

	//---------------------------------------------------------------------
	// ImGuiSystem singleton.
	//---------------------------------------------------------------------
	extern ImGuiSystem& GetImGuiSystem();

	//---------------------------------------------------------------------
	// ImGuiSystem
	//---------------------------------------------------------------------
	class ImGuiSystem
	{
	public:
		ImGuiSystem();

		/// <summary>
		/// Initializes the imgui system.
		/// </summary>
		bool Initialize(SDL_Window& a_Window, SDL_Renderer& a_Renderer);

		/// <summary>
		/// Destroys all imgui assets.
		/// </summary>
		bool Destroy();

		/// <summary>
		/// Renders the ImGui frame.
		/// </summary>
		void Render(SDL_Renderer& a_Renderer);

		void ProcessEvents(const SDL_Event& a_Event);
	private:
		/// <summary>
		/// Creates the ImGui context for SDL3.
		/// </summary>
		/// <returns>True if the context creation succeeds, otherwise false.</returns>
		bool CreateContextSDL3(SDL_Window& a_Window, SDL_Renderer& a_Renderer);

		/// <summary>
		/// Initializes the ImGui UI components.
		/// </summary>
		void CreateImGui();
	private:
		float m_fFontSize = 0.0f; /// Default font size for ImGui.

		LevelEditorWindow m_EditorWindow;
	};
}