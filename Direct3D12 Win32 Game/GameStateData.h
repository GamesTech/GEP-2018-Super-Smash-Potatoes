#pragma once
#include "pch.h"

//GEP:: Basic data of current GameState to communicate to all Game Objects

struct GameStateData
{
	Keyboard::State m_keyboardState;
	Keyboard::State m_prevKeyboardState;
	Mouse::State m_mouseState;

	//length of time since last frame
	float m_dt;
};
