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

}

void AudioManager::initAudioManager()
{
	//GEP::init Audio System
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);


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
	TS->SetVolume(1.5f);
	TS->Play();
	m_sounds.push_back(TS);
}


void AudioManager::playLoopAmbience()
{
	loop->SetVolume(0.1f);
	loop->Play();
	m_sounds.push_back(loop);
}

void AudioManager::loadAllSounds()
{
	TS = new TestSound(m_audEngine.get(), "Explo1");
	loop = new Loop(m_audEngine.get(), "NightAmbienceSimple_02");
}
