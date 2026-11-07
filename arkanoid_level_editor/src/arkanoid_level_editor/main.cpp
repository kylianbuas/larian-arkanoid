#include <memory>
#include <SDL3/SDL.h>

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

constexpr float UPSCALE = 3;

int main(int a_iNumArgs, char* a_aArgs[])
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Arkanoid Level Editor", 1200, 960, SDL_WINDOW_RESIZABLE);

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

	if (a_iNumArgs > 1)
	{
		EDITOR->SetLevelPath(a_aArgs[1]);
	}

	EDITOR->LoadTextures();

	const float width = EDITOR->GetContentWidth();
	const float height = EDITOR->GetContentHeight();
	const float resolution = width / height;

	SDL_SetWindowSize(window, static_cast<int>(width * UPSCALE), static_cast<int>(height * UPSCALE));
	SDL_SetWindowAspectRatio(window, resolution, resolution);
	SDL_SetRenderLogicalPresentation(
		renderer,
		width,
		height,
		SDL_LOGICAL_PRESENTATION_LETTERBOX
	);

	EDITOR->Initialize();

	bool running = true;

	while (running)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (!EDITOR->ProcessEvent(event))
			{
				running = false;
				break;
			}
		}

		if (!running)
		{
			break;
		}

		EDITOR->Render();
		SDL_RenderPresent(renderer);
	}

	EDITOR->Destroy();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
