#include "./GameAudioManager.h"

#include "arkanoid_game/game/Game.h"

namespace arkanoid::audio
{
	GameAudioManager::GameAudioManager(audio::AudioSystem& a_AudioSystem) :
		m_AudioSystem(a_AudioSystem)
	{}

	void GameAudioManager::Initialize()
	{
		const game::Game& game = game::GetGame();

		game.GetGameEvents().OnBallHitAnything += [this](const game::IBounceable*)
		{
			Snd_OnBallHitAnything();
		};

		game.GetGameEvents().OnBallHitBrick += [this](level::EBrickType)
		{
			Snd_OnBallHitBrick();
		};

		game.GetGameEvents().OnLifeLost += [this]()
		{
			Snd_OnLifeLost();
		};

		game.GetGameEvents().OnLifeAdded += [this]()
		{
			Snd_OnLifeAdded();
		};

		game.GetGameEvents().OnLevelComplete += [this]()
		{
			Snd_OnLifeAdded();
		};

		game.GetGameEvents().OnGameOver += [this]()
		{
			Snd_OnGameOver();
		};
	}

	void GameAudioManager::LoadSounds(resources::ResourceSystem& a_ResourceSystem)
	{
		a_ResourceSystem.LoadSound(resources::ESoundType::HIT, "snd_hit.wav");
		a_ResourceSystem.LoadSound(resources::ESoundType::HIT_BRICK, "snd_hit_brick.wav");
		a_ResourceSystem.LoadSound(resources::ESoundType::LEVEL_COMPLETE, "snd_level_complete.wav");
		a_ResourceSystem.LoadSound(resources::ESoundType::LIFE_LOST, "snd_life_lost.wav");
		a_ResourceSystem.LoadSound(resources::ESoundType::GAME_OVER, "snd_game_over.wav");
		a_ResourceSystem.LoadSound(resources::ESoundType::ADD_LIFE, "snd_add_life.wav");

		m_pBallHitSound = a_ResourceSystem.GetSound(resources::ESoundType::HIT);
		m_pBallHitBrick = a_ResourceSystem.GetSound(resources::ESoundType::HIT_BRICK);
		m_pLevelCompleteSound = a_ResourceSystem.GetSound(resources::ESoundType::LEVEL_COMPLETE);
		m_pLifeLostSound = a_ResourceSystem.GetSound(resources::ESoundType::LIFE_LOST);
		m_pGameOverSound = a_ResourceSystem.GetSound(resources::ESoundType::GAME_OVER);
		m_pLifeAddedSound = a_ResourceSystem.GetSound(resources::ESoundType::ADD_LIFE);
	}

	void GameAudioManager::Snd_OnBallHitAnything()
	{
		if (!m_pBallHitSound)
		{
			return;
		}
		
		m_AudioSystem.Play(*m_pBallHitSound);
	}

	void GameAudioManager::Snd_OnBallHitBrick()
	{
		if (!m_pBallHitBrick)
		{
			return;
		}
		
		m_AudioSystem.Play(*m_pBallHitBrick);
	}

	void GameAudioManager::Snd_OnLifeLost()
	{
		if (!m_pLifeLostSound)
		{
			return;
		}

		m_AudioSystem.Play(*m_pLifeLostSound);
	}

	void GameAudioManager::Snd_OnLifeAdded()
	{
		if (!m_pLifeAddedSound)
		{
			return;
		}

		m_AudioSystem.Play(*m_pLifeAddedSound);
	}

	void GameAudioManager::Snd_OnGameOver()
	{
		if (!m_pGameOverSound)
		{
			return;
		}

		m_AudioSystem.Play(*m_pGameOverSound);
	}

	void GameAudioManager::Snd_OnLevelComplete()
	{
		if (!m_pLevelCompleteSound)
		{
			return;
		}

		m_AudioSystem.Play(*m_pLevelCompleteSound);
	}
}