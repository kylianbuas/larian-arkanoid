#pragma once

namespace arkanoid::resources
{
	class ResourceSystem;
	class Sound;
}
namespace arkanoid::audio
{
	class AudioSystem;

	// Used for subscribing to game events and playing a sound, rather than hardcoding sounds in other classes.
	class GameAudioManager
	{
	public:
		GameAudioManager(audio::AudioSystem& a_AudioSystem);
		void Initialize();

		void LoadSounds(resources::ResourceSystem& a_ResourceSystem);
	private:
		void Snd_OnBallHitAnything();
		void Snd_OnBallHitBrick();
		void Snd_OnLifeLost();
		void Snd_OnLifeAdded();
		void Snd_OnGameOver();
		void Snd_OnLevelComplete();
		void Snd_OnPowerUpPickedUp();
		void Snd_OnShoot();

		resources::Sound* m_pBallHitSound = nullptr; // Not owned.
		resources::Sound* m_pBallHitBrick = nullptr; // Not owned.
		resources::Sound* m_pLifeLostSound = nullptr; // Not owned.
		resources::Sound* m_pLifeAddedSound = nullptr; // Not owned.
		resources::Sound* m_pGameOverSound = nullptr; // Not owned.
		resources::Sound* m_pLevelCompleteSound = nullptr; // Not owned.
		resources::Sound* m_pPowerUpPickedUpSound = nullptr; // Not owned.
		resources::Sound* m_pShootSound = nullptr; // Not owned.

		audio::AudioSystem& m_AudioSystem;
	};
}