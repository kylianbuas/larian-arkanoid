#pragma once

#include <SDL3/SDL_audio.h>
#include <vector>

namespace arkanoid::resources
{
	class Sound;
}
namespace arkanoid::audio
{
	class AudioSystem
	{
	public:
		AudioSystem();

		void Destroy();

		bool Play(const resources::Sound& a_Sound, float a_fPitch = 1.0f);
	private:
		void CleanupFinishedSounds();

		SDL_AudioSpec m_Spec = {};
		SDL_AudioDeviceID m_DeviceID = 0;
		std::vector<SDL_AudioStream*> m_aStreams;
	};
}