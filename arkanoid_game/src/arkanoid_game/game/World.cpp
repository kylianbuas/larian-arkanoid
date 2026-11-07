#include "./World.h"

namespace arkanoid::game
{
	void World::LoadTextures(resources::ResourceSystem& a_ResourceSystem)
	{
		m_pLevel->LoadTextures(a_ResourceSystem);
		m_pBall->LoadTextures(a_ResourceSystem);
		m_pPaddle->LoadTextures(a_ResourceSystem);
	}

	void World::Initialize()
	{
		m_pLevel = std::make_unique<level::Level>();
		m_pBall = std::make_unique<Ball>();
		m_pPaddle = std::make_unique<Paddle>();

		ResetWorld();
	}

	void World::ResetWorld()
	{
		m_aBounceables.clear();
		m_aConsumeables.clear();

		m_pLevel->Clear();

		m_pBall->Reset();
		m_pPaddle->Reset();

		m_aGameObjects.clear();
	}

	bool World::Update(float a_fDeltaTime)
	{
		std::erase_if(m_aGameObjects, [](const std::unique_ptr<IGameObject>& gameObject)
		{
			return gameObject->IsDestroyed();
		});

		const size_t numConsumeables = m_aGameObjects.size();
		for (int i = static_cast<int>(numConsumeables) - 1; i >= 0; i--)
		{
			std::unique_ptr<IGameObject>& gameObject = m_aGameObjects[i];
			if (!gameObject)
			{
				continue;
			}

			gameObject->Update(a_fDeltaTime);
		}

		m_pBall->Update(a_fDeltaTime); // First update the ball. The ball iterates through bounceables.

		m_pLevel->Update(a_fDeltaTime);
		m_pPaddle->Update(a_fDeltaTime);

		return m_pLevel->IsComplete();
	}

	void World::Render(SDL_Renderer& a_Renderer) const
	{
		m_pLevel->Render(a_Renderer);
		m_pPaddle->Render(a_Renderer);
		m_pBall->Render(a_Renderer);

		const size_t numConsumeables = m_aGameObjects.size();
		for (int i = static_cast<int>(numConsumeables) - 1; i >= 0; i--)
		{
			const std::unique_ptr<IGameObject>& gameObject = m_aGameObjects[i];
			if (!gameObject)
			{
				continue;
			}

			gameObject->Render(a_Renderer);
		}
	}

	const level::Level& World::GetLevel() const
	{
		return *m_pLevel;
	}

	level::Level& World::GetLevel()
	{
		return *m_pLevel;
	}

	const game::Ball& World::GetBall() const
	{
		return *m_pBall;
	}

	game::Ball& World::GetBall()
	{
		return *m_pBall;
	}

	const game::Paddle& World::GetPaddle() const
	{
		return *m_pPaddle;
	}

	game::Paddle& World::GetPaddle()
	{
		return *m_pPaddle;
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
		if (m_aBounceables.empty())
		{
			return;
		}

		std::erase(m_aBounceables, &a_Bounceable);
	}

    void World::AddConsumeable(IConsumeable& a_Consumeable)
    {
		if (std::find(m_aConsumeables.begin(), m_aConsumeables.end(), &a_Consumeable) == m_aConsumeables.end())
		{
			m_aConsumeables.push_back(&a_Consumeable);
		}
	}

	void World::RemoveConsumeable(IConsumeable & a_Consumeable)
	{
		std::erase(m_aConsumeables, &a_Consumeable);
	}

	const std::vector<IBounceable*>& World::GetBounceables() const
	{
		return m_aBounceables;
	}

	std::vector<IBounceable*>& World::GetBounceables()
	{
		return m_aBounceables;
	}

	const std::vector<IConsumeable*>& World::GetConsumeables() const
	{
		return m_aConsumeables;
	}

	std::vector<IConsumeable*>& World::GetConsumeables()
	{
		return m_aConsumeables;
	}
}