#include "./AudioSystem.h"

#include <SDL3/SDL.h>

#include <climits>
#include <vector>

#include "arkanoid_lib/resources/ResourceSystem.h"

namespace arkanoid::audio
{
	AudioSystem::AudioSystem()
	{
		m_Spec.format = SDL_AUDIO_F32;
		m_Spec.channels = 2;
		m_Spec.freq = 48000;

		m_DeviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_Spec);
		if (!m_DeviceID)
		{
			SDL_Log("Failed to open audio device: %s", SDL_GetError());
			return;
		}

		if (!SDL_ResumeAudioDevice(m_DeviceID))
		{
			SDL_Log("Failed to resume audio device: %s", SDL_GetError());
		}
	}

	void AudioSystem::Destroy()
	{
		if (m_DeviceID)
		{
			SDL_CloseAudioDevice(m_DeviceID);
			m_DeviceID = 0;
		}

		for (SDL_AudioStream* pStream : m_aStreams)
		{
			SDL_DestroyAudioStream(pStream);
		}

		m_aStreams.clear();
	}

	void AudioSystem::CleanupFinishedSounds()
	{
		for (size_t i = m_aStreams.size(); i > 0; i--)
		{
			SDL_AudioStream* pStream = m_aStreams[i - 1];

			if (SDL_GetAudioStreamQueued(pStream) == 0 && SDL_GetAudioStreamAvailable(pStream) == 0)
			{
				SDL_DestroyAudioStream(pStream);
				m_aStreams.erase(m_aStreams.begin() + i - 1);
			}
		}
	}

	bool AudioSystem::Play(const resources::Sound& a_Sound, float a_fPitch)
	{
		if (!m_DeviceID || !a_Sound.m_pData || a_Sound.m_iLength == 0 || a_Sound.m_iLength > INT_MAX)
		{
			return false;
		}

		CleanupFinishedSounds();

		SDL_AudioStream* pStream = SDL_CreateAudioStream(&a_Sound.m_Spec, &m_Spec);
		if (!pStream)
		{
			SDL_Log("Failed to create audio stream: %s", SDL_GetError());
			return false;
		}

		if (!SDL_SetAudioStreamFrequencyRatio(pStream, a_fPitch))
		{
			SDL_Log("Failed to set audio pitch: %s", SDL_GetError());
			SDL_DestroyAudioStream(pStream);
			return false;
		}

		if (!SDL_BindAudioStream(m_DeviceID, pStream))
		{
			SDL_Log("Failed to bind audio stream: %s", SDL_GetError());
			SDL_DestroyAudioStream(pStream);
			return false;
		}

		if (!SDL_PutAudioStreamData(pStream, a_Sound.m_pData, static_cast<int>(a_Sound.m_iLength)))
		{
			SDL_Log("Failed to queue sound: %s", SDL_GetError());
			SDL_DestroyAudioStream(pStream);
			return false;
		}

		if (!SDL_FlushAudioStream(pStream))
		{
			SDL_Log("Failed to flush audio stream: %s", SDL_GetError());
			SDL_DestroyAudioStream(pStream);
			return false;
		}

		m_aStreams.push_back(pStream);
		return true;
	}
}