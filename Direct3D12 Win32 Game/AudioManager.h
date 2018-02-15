#pragma once
#include "Audio.h"
#include <vector>

class AudioManager
{
public:
	AudioManager() = default;
	~AudioManager();
	void initAudioManager();
	void updateAudioManager(GameStateData* gsd);
	void suspendAudioManager();
	void resumeAudioManager();
	void playSound();
	void playLoopAmbience(int track);

private:
	void loadAllSounds();
	std::vector<Sound*> m_sounds;
	//audio system
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	std::vector<Loop*> loop_vector;
};

