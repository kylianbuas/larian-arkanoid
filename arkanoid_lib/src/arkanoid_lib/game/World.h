#pragma once

#include <vector>

#include "arkanoid_lib/game/Ball.h"
#include "arkanoid_lib/game/IBounceable.h"
#include "arkanoid_lib/game/Paddle.h"
#include "arkanoid_lib/level/Level.h"

struct SDL_Renderer;

namespace arkanoid::resources
{
	class ResourceSystem;
}
namespace arkanoid::game
{
	class World
	{
	public:
		void LoadTextures(resources::ResourceSystem& a_ResourceSystem);
		void LoadSounds(resources::ResourceSystem& a_ResourceSystem);
		void Initialize();

		void ResetWorld();

		bool Update(float a_fDeltaTime); // Returns true if level complete.
		void Render(SDL_Renderer& a_Renderer) const;

		const level::Level& GetLevel() const;
		level::Level& GetLevel();

		const game::Ball& GetBall() const;
		game::Ball& GetBall();

		const game::Paddle& GetPaddle() const;
		game::Paddle& GetPaddle();

		void AddBounceable(IBounceable& a_Bounceable);
		void RemoveBounceable(IBounceable& a_Bounceable);

		const std::vector<IBounceable*>& GetBounceables() const;
		std::vector<IBounceable*>& GetBounceables();
	private:
		level::Level m_Level; // Stores the bricks and other static world objects.
		game::Ball m_Ball;
		game::Paddle m_Paddle;
		std::vector<IBounceable*> m_aBounceables; // Non-owning.
	};
}