#include "./ImGuiSystem.h"

#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_sdlrenderer3.h>

namespace arkanoid::imgui
{
	ImGuiSystem& GetImGuiSystem()
	{
		static ImGuiSystem system;
		return system;
	}

	ImGuiSystem::ImGuiSystem()
	{}

	bool ImGuiSystem::Initialize(SDL_Window& a_Window, SDL_Renderer& a_Renderer)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		if (!CreateContextSDL3(a_Window, a_Renderer))
		{
			return false;
		}

		CreateImGui();

		m_EditorWindow.Initialize();

		return true;
	}

	bool ImGuiSystem::Destroy()
	{
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

		return true;
	}

	bool ImGuiSystem::CreateContextSDL3(SDL_Window& a_Window, SDL_Renderer& a_Renderer)
	{
		ImGui_ImplSDL3_InitForSDLRenderer(&a_Window, &a_Renderer);
		ImGui_ImplSDLRenderer3_Init(&a_Renderer);

		return true;
	}

	void ImGuiSystem::CreateImGui()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

		io.FontGlobalScale = 3;

		ImGui::StyleColorsDark();
	}

	void ImGuiSystem::Render(SDL_Renderer& a_Renderer)
	{
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		m_EditorWindow.Render();

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), &a_Renderer);
	}

	void ImGuiSystem::ProcessEvents(const SDL_Event& a_Event)
	{
		ImGui_ImplSDL3_ProcessEvent(&a_Event);
	}
}