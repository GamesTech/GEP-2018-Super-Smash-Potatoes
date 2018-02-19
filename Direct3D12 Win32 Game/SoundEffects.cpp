#include "pch.h"
#include "SoundEffects.h"
#include "GameStateData.h"


SoundEffects::SoundEffects(AudioEngine* _audEngine, string _filename) :Sound(_audEngine, _filename)
{
}


SoundEffects::~SoundEffects()
{
}

void SoundEffects::Tick(GameStateData * _GSD)
{
	Play();
}
