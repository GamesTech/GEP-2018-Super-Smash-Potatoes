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
	void playSound(int effect_no);
	void changeLoopTrack(int track);

private:
	void loadAllSounds();
	std::vector<Sound*> m_sounds;
	//audio system
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	std::vector<SoundEffects*> sound_effect_vector;
	std::vector<Loop*> loop_vector;

	int track_being_played = -1;
	int prev_track_played = -1;
};

