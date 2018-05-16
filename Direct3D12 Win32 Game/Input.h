#pragma once
namespace Inputs
{
	enum Inputs
	{
		CLEAR = 20,
		ESCAPE = 21,
		ENTER = 22,

		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
		A = 4, //jump
		X = 5, //punch
		B = 6, //special
		Y = 7,
		START = 8, //menu/pause
		LB = 9,
		RB = 10,
		DOWN_A = 11, //drop ledge
		UP_X = 12, //up punch
		DOWN_X = 13, //down slam
	};
};

namespace CurrentScene
{
	enum CurrentScene
	{
		START,
		MENU,
		LEVELEDITOR,
		SETTINGS,
		ARENA,
		CHARACTER,
		GAME,
		GAMEOVER
	};
};

class Input
{
public:
	Input();
	~Input();

	void init();
	void update(GameStateData* gsd);
	bool clearInput();

	void ResumeInput();
	void SuspendInput();

	CurrentScene::CurrentScene current_scene = CurrentScene::START;

	const static int MAX_PLAYERS = 4;
	Inputs::Inputs inputs[MAX_PLAYERS];

private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;

	std::unique_ptr<DirectX::GamePad> m_gamePad;
	DirectX::GamePad::ButtonStateTracker m_buttons;

	Keyboard::State keyboard_state;
	Keyboard::State prev_keyboard_state;

	GamePad::State GP_state[MAX_PLAYERS];
	GamePad::State prev_GP_state[MAX_PLAYERS];

	std::vector<bool> pressed;

	void getPressed();

	enum Pressed
	{
		W,
		A,
		S,
		D,

		UP,
		DOWN,
		LEFT,
		RIGHT,

		LEFT_CTRL,
		LEFT_SHIFT,
		LEFT_ALT,

		Q,
		E,

		X,
		Z,
		R,
		F,
		G,
		I,

		SPACE,
		ENTER,
		ESCAPE,
		BACK,

		PRESSED_MAX = BACK
	};
};