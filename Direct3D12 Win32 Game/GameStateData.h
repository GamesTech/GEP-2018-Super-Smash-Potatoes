#pragma once
#include "pch.h"

//GEP:: Basic data of current GameState to communicate to all Game Objects

struct GameStateData
{
	const static int MAX_PLAYERS = 4;

	int x_resolution;
	int y_resolution;

	int camera_view_width;
	int camera_view_height;

	int player_podium_position[4] = {0,0,0,0};
	int position_in_podium;

	int player_selected[MAX_PLAYERS];

	//length of time since last frame
	float m_dt;

	int no_players;
	int arena_selected;
};

