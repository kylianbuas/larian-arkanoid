#include "./World.h"

namespace arkanoid::game
{
	void World::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
	{
		m_Level.LoadTextures(a_ResourceSystem);
		m_Ball.LoadTextures(a_ResourceSystem);
		m_Paddle.LoadTextures(a_ResourceSystem);
	}

	void World::LoadSounds(resources::ResourceSystem& a_ResourceSystem)
	{
		m_Ball.LoadSounds(a_ResourceSystem);
	}

	void World::Initialize()
	{
		m_Level.Clear();

		ResetWorld();
	}

	void World::ResetWorld()
	{
		m_aBounceables.clear();
		m_Level.Clear();

		m_Ball.Reset();
		m_Paddle.Reset();
	}

	bool World::Update(float a_fDeltaTime)
	{
		m_Ball.Update(a_fDeltaTime); // First update the ball. The ball iterates through bounceables.

		m_Level.Update(a_fDeltaTime);
		m_Paddle.Update(a_fDeltaTime);

		return m_Level.IsComplete();
	}

	void World::Render(SDL_Renderer& a_Renderer) const
	{
		m_Level.Render(a_Renderer);
		m_Paddle.Render(a_Renderer);
		m_Ball.Render(a_Renderer);
	}

	const level::Level& World::GetLevel() const
	{
		return m_Level;
	}

	level::Level& World::GetLevel()
	{
		return m_Level;
	}

	const game::Ball& World::GetBall() const
	{
		return m_Ball;
	}

	game::Ball& World::GetBall()
	{
		return m_Ball;
	}

	const game::Paddle& World::GetPaddle() const
	{
		return m_Paddle;
	}

	game::Paddle& World::GetPaddle()
	{
		return m_Paddle;
	}

	void World::AddBounceable(IBounceable& a_Bounceable)
	{
		if (std::find(m_aBounceables.begin(), m_aBounceables.end(), &a_Bounceable) == m_aBounceables.end())
		{
			m_aBounceables.push_back(&a_Bounceable);
		}
	}

	void World::RemoveBounceable(IBounceable & a_Bounceable)
	{
		// Set the pointer in the vector to null and erase it in the next update.
		std::vector<IBounceable*>::iterator it = std::find(m_aBounceables.begin(), m_aBounceables.end(), &a_Bounceable);
		if (it != m_aBounceables.end())
		{
			*it = nullptr;
		}
	}

	const std::vector<IBounceable*>& World::GetBounceables() const
	{
		return m_aBounceables;
	}

	std::vector<IBounceable*>& World::GetBounceables()
	{
		return m_aBounceables;
	}
}