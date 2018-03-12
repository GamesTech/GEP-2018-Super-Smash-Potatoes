#pragma once
#include "Sound.h"

//GEP:: An example sound which goes off when you press space.

class SoundEffects :
	public Sound
{
public:
	SoundEffects(AudioEngine* _audEngine, string _filename);
	~SoundEffects();

	virtual void Tick(GameStateData* _GSD);
};

