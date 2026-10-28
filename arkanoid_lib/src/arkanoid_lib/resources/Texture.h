#pragma once

struct SDL_Texture;

namespace arkanoid::resources
{
	class Texture
	{
	public:
		~Texture();

		SDL_Texture* m_pTexture = nullptr;
	};
}