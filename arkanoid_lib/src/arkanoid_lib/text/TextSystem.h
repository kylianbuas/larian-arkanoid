#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "arkanoid_lib/resources/Texture.h"

struct SDL_Renderer;
struct TTF_Font;

namespace arkanoid::text
{
	class Font
	{
	public:
		~Font();
		TTF_Font* m_pFont = nullptr;
	};

	class TextSystem
	{
	public:
		TextSystem(SDL_Renderer& a_Renderer);

		void Destroy();

		bool Initialize();

		void DisplayText(const std::string& a_sText, size_t a_iPosX, size_t a_iPosY);
		void GetTextSize(const std::string& a_sText, int& a_fWidth, int& a_fHeight) const;
	private:
		SDL_Renderer& m_Renderer;
		std::unique_ptr<Font> m_pFont;
		std::unordered_map<std::string, resources::Texture> m_mTextTextures;
	};
}