#pragma once

#include <functional>

namespace arkanoid::level
{
	enum class EBrickType : uint8_t;
}
namespace arkanoid::game
{
	template<typename... Args>
	class Event
	{
	public:
		using Callback = std::function<void(Args...)>;

		void operator+=(Callback a_Callback) const
		{
			m_Callbacks.push_back(std::move(a_Callback));
		}

		void operator()(Args... a_Args)
		{
			for (const auto& callback : m_Callbacks)
				callback(a_Args...);
		}
	private:
		mutable std::vector<Callback> m_Callbacks; // Mutable because const should still be possible to be subscribed to.
	};

	class IBounceable;

	class GameEvents
	{
	public:
		Event<> OnGameOver;
		Event<> OnLevelComplete;
		Event<> OnLifeLost;
		Event<> OnLifeAdded;
		Event<const IBounceable*> OnBallHitAnything;
		Event<level::EBrickType> OnBallHitBrick;
	};
}