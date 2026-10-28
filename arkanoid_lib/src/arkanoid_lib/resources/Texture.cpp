#include "./Texture.h"

#include <SDL3/SDL_render.h>

namespace arkanoid::resources
{
	Texture::~Texture()
	{
		SDL_DestroyTexture(m_pTexture);
	}
}