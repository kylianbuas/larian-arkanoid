#pragma once

#include "arkanoid_lib/resources/ResourceSystem.h"

struct SDL_Window;
struct SDL_Renderer;

namespace arkanoid::editor
{
	class Editor;
	extern Editor& GetEditor();

	class Editor
	{
	public:
		Editor(SDL_Window& a_Window, SDL_Renderer& a_Renderer);

		bool LoadTextures();

		SDL_Window& GetWindow();
		SDL_Renderer& GetRenderer();

		resources::ResourceSystem& GetResourceSystem();
	private:
		SDL_Window& m_Window;
		SDL_Renderer& m_Renderer;
		resources::ResourceSystem m_ResourceSystem;
	};
}