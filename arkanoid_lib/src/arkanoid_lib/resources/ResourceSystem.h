#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <SDL3\SDL_audio.h>
#include <string>

#include "arkanoid_lib/level/LevelData.h"
#include "arkanoid_lib/resources/FilePCH.h"
#include "arkanoid_lib/resources/Texture.h"

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_AudioStream;

namespace arkanoid::resources
{
	enum class ETextureType
	{
		DUMMY,
		BG,
		BRICK,
		BRICK_SHINE_OVERLAY,
		PADDLE,
		BALL,
		EXPLOSION,
		MAX = EXPLOSION
	};

	enum class ESoundType
	{
		HIT,
		HIT_BRICK,
		LEVEL_COMPLETE,
		LIFE_LOST,
		GAME_OVER,
		MAX = GAME_OVER
	};

	class Sound
	{
	public:
		~Sound();

		SDL_AudioSpec m_Spec = {};
		uint8_t* m_pData = nullptr;
		uint32_t m_iLength = 0;
	};

	class ResourceSystem
	{
	public:
		ResourceSystem(SDL_Renderer& a_Renderer);

		void Destroy();

		bool LoadTexture(ETextureType a_eType, const fs::path& a_sPath);
		SDL_Texture* GetTexture(ETextureType a_eType);

		level::LevelData LoadLevel(const fs::path& a_sPath);

		bool LoadSound(ESoundType a_eType, const fs::path& a_sPath);
		Sound* GetSound(ESoundType a_eType);
	private:
		SDL_Renderer& m_Renderer;
		std::array<std::unique_ptr<Texture>, static_cast<size_t>(ETextureType::MAX) + 1> m_aTextures = {};
		std::array<std::unique_ptr<Sound>, (int) ESoundType::MAX + 1> m_aSounds = {};
	};
}