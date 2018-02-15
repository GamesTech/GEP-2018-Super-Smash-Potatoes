#include "pch.h"
#include "AudioManager.h"

AudioManager::~AudioManager()
{
	if (m_audEngine)
	{
		m_audEngine->Suspend();
	}
	for (std::vector<Sound *>::iterator it = m_sounds.begin(); it != m_sounds.end(); it++)
	{
		delete (*it);
	}
	m_sounds.clear();
	loop_vector.clear();

}

void AudioManager::initAudioManager()
{
	//GEP::init Audio System
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);

	loadAllSounds();
}

void AudioManager::updateAudioManager(GameStateData* gsd)
{
	//this will update the audio engine but give us chance to do somehting else if that isn't working
	if (!m_audEngine->Update())
	{
		if (m_audEngine->IsCriticalError())
		{
			// We lost the audio device!
		}
	}
	else
	{
		//update sounds playing
		for (std::vector<Sound *>::iterator it = m_sounds.begin(); it != m_sounds.end(); it++)
		{
			(*it)->Tick(gsd);
		}
	}
}

void AudioManager::suspendAudioManager()
{
	m_audEngine->Suspend();
}

void AudioManager::resumeAudioManager()
{
	m_audEngine->Resume();
}

void AudioManager::playSound()
{
	//m_sounds.push_back(new SoundAndMusic(m_audEngine.get(), "Explo1"));
}


void AudioManager::playLoopAmbience(int track)
{
	if (track < loop_vector.size())
	{
		m_sounds.push_back(loop_vector[track]);
	}
}

void AudioManager::loadAllSounds()
{
	m_sounds.push_back(new SoundAndMusic(m_audEngine.get(), "Explo1"));

	loop_vector.push_back(new Loop(m_audEngine.get(), "NightAmbienceSimple_02"));
}
