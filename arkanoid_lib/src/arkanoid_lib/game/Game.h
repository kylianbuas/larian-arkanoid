#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <string>

#include "arkanoid_lib/audio/AudioSystem.h"
#include "arkanoid_lib/game/Ball.h"
#include "arkanoid_lib/game/Paddle.h"
#include "arkanoid_lib/game/World.h"
#include "arkanoid_lib/level/Level.h"
#include "arkanoid_lib/level/LevelManager.h"
#include "arkanoid_lib/resources/ResourceSystem.h"
#include "arkanoid_lib/text/TextSystem.h"

struct SDL_Window;
struct SDL_Renderer;

namespace arkanoid::game
{
	class IBounceable;

	class Game;
	extern Game& GetGame();

	class Game
	{
	public:
		Game(SDL_Window& a_Window, SDL_Renderer& a_Renderer);

		void Initialize();
		void LoadTextures();
		void LoadSounds();

		void Destroy();

		// Game width and height. Used for game logic.
		float GetDummyWidth() const; // Used for if textures do not work.
		float GetDummyHeight() const; // Used for if textures do not work.
		float GetGameWidth() const;
		float GetGameHeight() const;

		void Update(float a_fDeltaTime);
		void Render();

		const resources::ResourceSystem& GetResourceSystem() const;
		resources::ResourceSystem& GetResourceSystem();

		const audio::AudioSystem& GetAudioSystem() const;
		audio::AudioSystem& GetAudioSystem();

		const text::TextSystem& GetTextSystem() const;
		text::TextSystem& GetTextSystem();

		const level::LevelManager& GetLevelManager() const;
		level::LevelManager& GetLevelManager();

		const game::World& GetWorld() const;
		game::World& GetWorld();

		void ResetLives();
		void LoseLife();
	private:
		resources::Sound* m_pLevelCompleteSound = nullptr; // Not owned.
		resources::Sound* m_pLifeLostSound = nullptr; // Not owned.
		resources::Sound* m_pGameOverSound = nullptr; // Not owned.
		size_t m_iLives = 0;

		SDL_Window& m_Window;
		SDL_Renderer& m_Renderer;
		resources::ResourceSystem m_ResourceSystem; // Responsible for loading textures, level data and sfx.
		audio::AudioSystem m_AudioSystem;
		text::TextSystem m_TextSystem;
		level::LevelManager m_LevelManager; // Responsible for storing the level state (level index, loading next level).

		World m_World;

		float m_fGameWidth = 0.0f; // Game width (used in logic).
		float m_fGameHeight = 0.0f; // Game height (used in logic).

		float m_fDummyWidth = 0; // Width of a brick.
		float m_fDummyHeight = 0; // Height of a brick.
	};
}