#pragma once

#include <memory>
#include <vector>

#include "arkanoid_lib/game/Brick.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace arkanoid::resources
{
	class ResourceSystem;
}
namespace arkanoid::level
{
	class LevelData;

	class Level
	{
	public:
		bool LoadTextures(resources::ResourceSystem& a_ResourceSystem);
		bool Initialize(const LevelData& a_LevelData);
		void Clear();

		bool IsComplete() const;

		std::vector<game::Brick>& GetBricks();
		const std::vector<game::Brick>& GetBricks() const;

		void Shine(float a_fDeltaTime);
		void Update(float a_fDeltaTime);

		void Render(SDL_Renderer& a_Renderer) const;
	private:
		std::vector<game::Brick> m_aBricks; 

		SDL_Texture* m_pBrickTexture = nullptr; // Not owned.
		SDL_Texture* m_pBrickShineTexture = nullptr; // Not owned.

		float m_fShineTime = 0.0f;
		const float m_fShineChange = 0.15f;
		int m_iShineIndex = 0;
		int m_iShineMax = 5;
		float m_fShineDelay = 3.0f;
		float m_fShineDelayTime = 0.0f;

		float m_fBrickWidth = 0;
		float m_fBrickHeight = 0;
	};
}