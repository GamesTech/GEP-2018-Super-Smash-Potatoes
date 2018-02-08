#pragma once

enum State {
	PLAY = 1,
	PAUSE = 0,
	MENU = 2,
	EXIT = -1,
};

struct GameState {
	public:
		static State state;
};