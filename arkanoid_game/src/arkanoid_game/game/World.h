#pragma once

#include <vector>

#include "arkanoid_game/game/objects/Ball.h"
#include "arkanoid_game/game/objects/Paddle.h"
#include "arkanoid_game/level/Level.h"

struct SDL_Renderer;

namespace arkanoid::resources
{
	class ResourceSystem;
}
namespace arkanoid::game
{
	class IBounceable;
	class IConsumeable;

	class World
	{
	public:
		void Initialize();
		void LoadTextures(resources::ResourceSystem& a_ResourceSystem);

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

		void AddConsumeable(IConsumeable& a_Consumeable);
		void RemoveConsumeable(IConsumeable& a_Consumeable);

		const std::vector<IBounceable*>& GetBounceables() const;
		std::vector<IBounceable*>& GetBounceables();

		const std::vector<IConsumeable*>& GetConsumeables() const;
		std::vector<IConsumeable*>& GetConsumeables();

		template<typename T, typename... Args>
		void SpawnGameObject(Args&&... a_Args)
		{
			m_aGameObjects.emplace_back(
				std::make_unique<T>(std::forward<Args>(a_Args)...)
			);
		}
	private:
		std::unique_ptr<level::Level> m_pLevel; // Stores the bricks and other static world objects.
		std::unique_ptr<game::Ball> m_pBall; // Separate from game objects.
		std::unique_ptr<game::Paddle> m_pPaddle; // Separate from game objects.

		std::vector<IBounceable*> m_aBounceables; // Non-owning.
		std::vector<IConsumeable*> m_aConsumeables; // Non-owning.

		std::vector<std::unique_ptr<IGameObject>> m_aGameObjects; // Owns the objects.
	};
}