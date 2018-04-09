#pragma once
#include "pch.h"

//GEP:: Basic data of current GameState to communicate to all Game Objects

enum GameState
{
	MENU,
	CHARACTERSELECT,
	ARENASELECT,
	SETTINGS,
	INGAME,
	GAMEOVER
};

enum Winner
{
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4,
};

struct GameStateData
{
	Keyboard::State m_keyboardState;
	Keyboard::State m_prevKeyboardState;
	Mouse::State m_mouseState;

	GamePad::State m_gamePadState[4];
	GamePad::State m_prevGamePadState[4];

	static GameState state;
	static Winner winner;

	int player_selected[4];

	//length of time since last frame
	float m_dt;

	int no_players;
	int arena_selected;

	GameState gameState;
	Winner winState;
};

