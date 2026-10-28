#include "Editor.h"

#include <SDL.h>

namespace arkanoid::editor
{
	Editor::Editor(SDL_Window& a_Window, SDL_Renderer& a_Renderer) :
		m_Window(a_Window),
		m_Renderer(a_Renderer),
		m_ResourceSystem(a_Renderer)
	{
		SDL_Log("Working directory: %s", SDL_GetCurrentDirectory());
	}

	bool Editor::LoadTextures()
	{
		if (!m_ResourceSystem.LoadTexture(resources::ETextureType::BRICK, "tex_bricks.png"))
		{
			return false;
		}
		return true;
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
}
