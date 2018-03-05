#pragma once
#include "pch.h"

//GEP:: Basic data of current GameState to communicate to all Game Objects

enum GameState
{
	MENU,
	SETTINGS,
	INGAME,
	GAMEOVER
};

struct GameStateData
{
	Keyboard::State m_keyboardState;
	Keyboard::State m_prevKeyboardState;
	Mouse::State m_mouseState;

	GamePad::State m_gamePadState[4];
	GamePad::State m_prevGamePadState[4];

	static GameState state;

	//length of time since last frame
	float m_dt;
	int no_players;

	GameState gameState;
};

