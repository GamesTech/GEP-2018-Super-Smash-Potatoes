#pragma once
#include "Audio.h"
#include <vector>

enum Track
{
	EXPLOSION,
	NIGHTAMBIENCE,
	TOBYSOUNDTRACK,
	TOBYSOUNDTRACK2,
	TOBYSOUNDTRACK3,
	TOBYMENUCLICK1,
	QUESTCOMPLETE,
	SLAPSOUND
};

class AudioManager
{
public:
	AudioManager() = default;
	~AudioManager();
	void initAudioManager();
	void updateAudioManager(GameStateData* gsd);
	void suspendAudioManager();
	void resumeAudioManager();
	void playSound(Track track);
	void changeLoopTrack(Track track);

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

