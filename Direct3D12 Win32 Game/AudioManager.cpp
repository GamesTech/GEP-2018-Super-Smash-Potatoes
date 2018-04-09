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
		for (int i = 0; i < m_sounds.size(); i++)
		{
			m_sounds[i]->Tick(gsd);
			if (m_sounds[i]->GetPlaying() == false)
			{
				m_sounds.erase(m_sounds.begin() + i);
			}
		}

		//Track Transition
		if (prev_track_played > -1)
		{
			if (loop_vector[prev_track_played]->GetVolume() > 0)
			{
				loop_vector[prev_track_played]->SetVolume(loop_vector[prev_track_played]->GetVolume() - 0.0015f);
			}
		}
		if (track_being_played > -1)
		{
			if (loop_vector[track_being_played]->GetVolume() < 1)
			{
				loop_vector[track_being_played]->SetVolume(loop_vector[track_being_played]->GetVolume() + 0.001f);
			}
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

void AudioManager::playSound(Track track)
{
	int effect_no = track;
	if (effect_no < sound_effect_vector.size())
	{
		m_sounds.push_back(sound_effect_vector[effect_no]);
	}
}


void AudioManager::changeLoopTrack(Track track)
{
	int loop_track = track;
	if (loop_track < loop_vector.size() && loop_track != track_being_played)
	{
		if (track_being_played > -1)
		{
			prev_track_played = track_being_played;
		}

		loop_vector[loop_track]->Play();
		//if not first track
		if (track_being_played != -1)
		{
			loop_vector[loop_track]->SetVolume(0);
		}
		m_sounds.push_back(loop_vector[loop_track]);

		track_being_played = loop_track;
	}
}

void AudioManager::loadAllSounds()
{
	sound_effect_vector.push_back(new SoundEffects(m_audEngine.get(), "Explo1"));
	sound_effect_vector.push_back(new SoundEffects(m_audEngine.get(), "NightAmbienceSimple_02"));
	sound_effect_vector.push_back(new SoundEffects(m_audEngine.get(), "TobySoundtrack"));
	sound_effect_vector.push_back(new SoundEffects(m_audEngine.get(), "TobySoundtrack2"));
	sound_effect_vector.push_back(new SoundEffects(m_audEngine.get(), "TobySoundtrack3"));
	sound_effect_vector.push_back(new SoundEffects(m_audEngine.get(), "TobyMenuClick1"));
	sound_effect_vector.push_back(new SoundEffects(m_audEngine.get(), "Quest_Completion"));
	sound_effect_vector.push_back(new SoundEffects(m_audEngine.get(), "SlapSound"));

	loop_vector.push_back(new Loop(m_audEngine.get(), "Explo1"));
	loop_vector.push_back(new Loop(m_audEngine.get(), "NightAmbienceSimple_02"));
	loop_vector.push_back(new Loop(m_audEngine.get(), "TobySoundtrack"));
	loop_vector.push_back(new Loop(m_audEngine.get(), "TobySoundtrack2"));
	loop_vector.push_back(new Loop(m_audEngine.get(), "TobySoundtrack3"));
	loop_vector.push_back(new Loop(m_audEngine.get(), "TobyMenuClick1"));
	loop_vector.push_back(new Loop(m_audEngine.get(), "Quest_Completion"));
	loop_vector.push_back(new Loop(m_audEngine.get(), "SlapSound"));


	//currently needs to duplicate like this due to currnet ENUM system
}
