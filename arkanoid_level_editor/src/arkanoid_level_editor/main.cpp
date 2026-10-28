#include <memory>
#include <SDL3/SDL.h>

#include "arkanoid_level_editor/imgui/ImGuiSystem.h"
#include "arkanoid_level_editor/editor/Editor.h"

using namespace arkanoid;

std::unique_ptr<editor::Editor> EDITOR;

namespace arkanoid::editor
{
	Editor& GetEditor()
	{
		return *EDITOR;
	}
}

int main(int a_iNumArgs, char* a_aArgs[])
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Arkanoid", 1000, 1000, SDL_WINDOW_RESIZABLE);

	if (!window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	if (!renderer)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	EDITOR = std::make_unique<editor::Editor>(*window, *renderer);
	if (!EDITOR->LoadTextures())
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	imgui::ImGuiSystem imguiSystem;
	if (!imguiSystem.Initialize(*window, *renderer))
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	bool running = true;

	while (running)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				running = false;
			}
			imguiSystem.ProcessEvents(event);
		}

		SDL_SetRenderDrawColor(renderer, 15, 20, 35, 255);
		SDL_RenderClear(renderer);

		imguiSystem.Render(*renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}