#pragma once
#include "SoundAndMusic.h"

//GEP:: An example sound which goes off when you press space.

class SoundAndMusic :
	public Sound
{
public:
	SoundAndMusic(AudioEngine* _audEngine, string _filename);
	~SoundAndMusic();

	virtual void Tick(GameStateData* _GSD);
};

