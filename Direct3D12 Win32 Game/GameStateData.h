#pragma once
#include "pch.h"

//GEP:: Basic data of current GameState to communicate to all Game Objects

enum Winner
{
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4,
};

struct GameStateData
{
	const static int MAX_PLAYERS = 4;

	Keyboard::State m_keyboardState;
	Keyboard::State m_prevKeyboardState;
	Mouse::State m_mouseState;

	GamePad::State m_gamePadState[MAX_PLAYERS];
	GamePad::State m_prevGamePadState[MAX_PLAYERS];

	int x_resolution;
	int y_resolution;

	int camera_view_width;
	int camera_view_height;

	static Winner winner;

	int player_selected[MAX_PLAYERS];

	//length of time since last frame
	float m_dt;

	int no_players;
	int arena_selected;

	Winner winState;
};

