#pragma once

#include <memory>
#include <vector>

#include "arkanoid_game/game/objects/Brick.h"

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

		// Bricks are heap allocated one by one: World::m_aBounceables stores raw pointers to them,
		// so a brick must never move inside the vector (erasing/inserting would dangle those pointers).
		std::vector<std::unique_ptr<game::Brick>>& GetBricks();
		const std::vector<std::unique_ptr<game::Brick>>& GetBricks() const;

		void Update(float a_fDeltaTime);

		void Render(SDL_Renderer& a_Renderer) const;
	private:
		std::vector<std::unique_ptr<game::Brick>> m_aBricks; // Owns the bricks.

		SDL_Texture* m_pBrickTexture = nullptr; // Not owned.

		float m_fBrickWidth = 0;
		float m_fBrickHeight = 0;
	};
}