#pragma once
namespace Inputs
{
	enum Inputs
	{
		CLEAR,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		A, //jump
		X, //punch
		B, //special
		Y,
		START, //menu/pause
		LEFT_TRIGGER,
		RIGHT_TRIGGER,
		DOWN_A, //drop ledge
		UP_X, //up punch
		DOWN_X, //down slam
	};
};

class Input
{
public:
	Input();
	~Input();

	void init();
	void update(GameStateData* gsd);
	void clearInput();

	void ResumeInput();
	void SuspendInput();

	const static int MAX_PLAYERS = 4;
	Inputs::Inputs inputs[MAX_PLAYERS];

private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;

	std::unique_ptr<DirectX::GamePad> m_gamePad;
	DirectX::GamePad::ButtonStateTracker m_buttons;

	Keyboard::State m_keyboardState;
	Keyboard::State m_prevKeyboardState;

	GamePad::State m_gamePadState[MAX_PLAYERS];
	GamePad::State m_prevGamePadState[MAX_PLAYERS];
};