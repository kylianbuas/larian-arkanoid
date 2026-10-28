#include "ResourceSystem.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_render.h>

namespace arkanoid::resources
{
	Sound::~Sound()
	{
		if (m_pData)
		{
			SDL_free(m_pData);
			m_pData = nullptr;
		}
	}

	ResourceSystem::ResourceSystem(SDL_Renderer& a_Renderer) :
		m_Renderer(a_Renderer)
	{}

	void ResourceSystem::Destroy()
	{
		for (std::unique_ptr<Texture>& texture : m_aTextures)
		{
			texture.reset();
		}
		for (std::unique_ptr<Sound>& sound : m_aSounds)
		{
			sound.reset();
		}
	}

	const fs::path TEX_DIR = "assets/textures";
	bool ResourceSystem::LoadTexture(ETextureType a_eType, const fs::path& a_sPath)
	{
		const fs::path loadPath = TEX_DIR / a_sPath;
		std::unique_ptr<Texture>& texture = m_aTextures[static_cast<size_t>(a_eType)];
		if (texture)
		{
			return true;
		}

		texture = std::make_unique<Texture>();

		texture->m_pTexture = IMG_LoadTexture(&m_Renderer, loadPath.string().c_str());
		if (!texture->m_pTexture)
		{
			SDL_Log("Failed to load texture \"%s\": %s", loadPath.string().c_str(), SDL_GetError());
			texture.reset();
			return false;
		}
		SDL_SetTextureScaleMode(texture->m_pTexture, SDL_SCALEMODE_PIXELART);

		return texture->m_pTexture;
	}

	SDL_Texture* ResourceSystem::GetTexture(ETextureType a_eType)
	{
		const std::unique_ptr<Texture>& texture = m_aTextures[static_cast<size_t>(a_eType)];
		if (!texture)
		{
			return nullptr;
		}
		return texture->m_pTexture;
	}

	const fs::path LEVEL_DIR = "assets/levels";
	level::LevelData ResourceSystem::LoadLevel(const fs::path& a_sPath)
	{
		level::LevelData levelData;

		const fs::path loadPath = LEVEL_DIR / a_sPath;

		FILE* file = nullptr;
		fopen_s(&file, loadPath.string().c_str(), "rb");

		if (!file)
		{
			return levelData;
		}

		fseek(file, 0, SEEK_END);
		const long fileSize = ftell(file);
		rewind(file);

		if (fileSize < 0 || fileSize > level::LEVEL_WIDTH * level::LEVEL_HEIGHT)
		{
			fclose(file);
			return levelData;
		}

		std::vector<uint8_t> fileData(fileSize);
		fread(fileData.data(), sizeof(uint8_t), fileSize, file);

		levelData.Load(fileData);

		fclose(file);

		return levelData;
	}

	const fs::path SND_DIR = "assets/sounds";
    bool ResourceSystem::LoadSound(ESoundType a_eType, const fs::path& a_sPath)
    {
		const fs::path loadPath = SND_DIR / a_sPath;
		std::unique_ptr<Sound>& sound = m_aSounds[static_cast<size_t>(a_eType)];
		if (sound)
		{
			return true;
		}

		sound = std::make_unique<Sound>();

		if (!SDL_LoadWAV(loadPath.string().c_str(), &sound->m_Spec, &sound->m_pData, &sound->m_iLength))
		{
			SDL_Log("Failed to load sound \"%s\": %s", loadPath.string().c_str(), SDL_GetError());
			sound.reset();
			return false;
		}

		return true;
    }

	Sound* ResourceSystem::GetSound(ESoundType a_eType)
	{
		return m_aSounds[static_cast<size_t>(a_eType)].get();
	}
}