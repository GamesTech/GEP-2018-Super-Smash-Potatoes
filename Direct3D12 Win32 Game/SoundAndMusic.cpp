#include "pch.h"
#include "SoundAndMusic.h"
#include "GameStateData.h"


SoundAndMusic::SoundAndMusic(AudioEngine* _audEngine, string _filename) :Sound(_audEngine, _filename)
{
}


SoundAndMusic::~SoundAndMusic()
{
}

void SoundAndMusic::Tick(GameStateData * _GSD)
{
	if (_GSD->m_keyboardState.Space && !_GSD->m_prevKeyboardState.Space)
	{
		Play();
	}
}
