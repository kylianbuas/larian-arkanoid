#include <SDL3/SDL.h>

#include "arkanoid_lib/game/Game.h"
#include "arkanoid_lib/level/BrickTypes.h"

using namespace arkanoid;

std::unique_ptr<game::Game> GAME;

namespace arkanoid::game
{
	Game& GetGame()
	{
		return *GAME;
	}
}

constexpr float UPSCALE = 3;

int main(int a_iNumArgs, char* a_aArgs[])
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Arkanoid", 1920, 1080, SDL_WINDOW_RESIZABLE);

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

	GAME = std::make_unique<game::Game>(*window, *renderer);
	GAME->LoadTextures();
	GAME->LoadSounds();

	const float width = GAME->GetGameWidth();
	const float height = GAME->GetGameHeight();

	float resolution = width / height;

	SDL_SetWindowSize(window, width * UPSCALE, height * UPSCALE);
	SDL_SetWindowAspectRatio(window, resolution, resolution);
	SDL_SetRenderLogicalPresentation(
		renderer,
		width,
		height,
		SDL_LOGICAL_PRESENTATION_LETTERBOX
	);
	SDL_SetWindowAspectRatio(window, resolution, resolution);

	GAME->GetLevelManager().SetLevels({
		"1.lvl",
		"2.lvl",
		"3.lvl",
		"4.lvl",
		"5.lvl",
		"6.lvl",
		"7.lvl",
	});
	GAME->Initialize();

	bool running = true;
	Uint64 previousTime = SDL_GetPerformanceCounter();

	while (running)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				running = false;
			}
		}

		Uint64 currentTime = SDL_GetPerformanceCounter();
		float deltaTime = static_cast<float>(
			static_cast<double>(currentTime - previousTime) /
			static_cast<double>(SDL_GetPerformanceFrequency()));

		previousTime = currentTime;

		GAME->Update(deltaTime);

		GAME->Render();
		SDL_RenderPresent(renderer);
	}

	GAME->Destroy();

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}