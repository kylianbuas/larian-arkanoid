#include "./TextSystem.h"

#include <SDL3_ttf/SDL_ttf.h>

namespace arkanoid::text
{
	Font::~Font()
	{
		if (m_pFont)
		{
			TTF_CloseFont(m_pFont);
		}
	}

	TextSystem::TextSystem(SDL_Renderer& a_Renderer) :
		m_Renderer(a_Renderer)
	{
		if (!TTF_Init())
		{
			SDL_Log("Failed to initialize SDL_ttf: %s", SDL_GetError());
			return;
		}

		m_pFont = std::make_unique<Font>();
		m_pFont->m_pFont = TTF_OpenFont("assets/text/font.ttf", 12.0f);

		if (!m_pFont->m_pFont)
		{
			SDL_Log("Failed to load font: %s", SDL_GetError());
		}
	}

	void TextSystem::Destroy()
	{
		m_pFont.reset();
		m_mTextTextures.clear();
	}

	void TextSystem::DisplayText(const std::string& a_sText, size_t a_iPosX, size_t a_iPosY)
	{
		if (!m_pFont || !m_pFont->m_pFont)
		{
			return;
		}

		std::unordered_map<std::string, resources::Texture>::iterator it = m_mTextTextures.find(a_sText);

		if (it == m_mTextTextures.end())
		{
			it = m_mTextTextures.try_emplace(a_sText).first;

			SDL_Color color = { 255, 255, 255, 255 };
			SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->m_pFont, a_sText.c_str(), 0, color);

			if (!pSurface)
			{
				SDL_Log("Failed to render text: %s", SDL_GetError());
				return;
			}

			SDL_Texture* pTexture = SDL_CreateTextureFromSurface(&m_Renderer, pSurface);
			SDL_DestroySurface(pSurface);

			if (!pTexture)
			{
				SDL_Log("Failed to create text texture: %s", SDL_GetError());
				return;
			}

			SDL_SetTextureScaleMode(pTexture, SDL_SCALEMODE_NEAREST);

			it->second.m_pTexture = pTexture;
		}

		float width = 0.0f;
		float height = 0.0f;

		if (!SDL_GetTextureSize(it->second.m_pTexture, &width, &height))
		{
			SDL_Log("Failed to get text texture size: %s", SDL_GetError());
			return;
		}

		SDL_FRect dstRect = {
			static_cast<float>(a_iPosX),
			static_cast<float>(a_iPosY),
			width,
			height
		};

		SDL_RenderTexture(&m_Renderer, it->second.m_pTexture, nullptr, &dstRect);
	}

	void TextSystem::GetTextSize(const std::string& a_sText, int& a_fWidth, int& a_fHeight) const
	{
		if (!m_pFont || !m_pFont->m_pFont)
		{
			a_fWidth = 0;
			a_fHeight = 0;
			return;
		}

		if (!TTF_GetStringSize(m_pFont->m_pFont, a_sText.c_str(), 0, &a_fWidth, &a_fHeight))
		{
			SDL_Log("Failed to calculate text size: %s", SDL_GetError());
			a_fWidth = 0;
			a_fHeight = 0;
		}
	}
}