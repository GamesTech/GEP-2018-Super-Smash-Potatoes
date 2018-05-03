#include "pch.h"
#include "Input.h"
#include "GameStateData.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::init()
{
	m_keyboard = std::make_unique<Keyboard>();
	m_gamePad = std::make_unique<GamePad>();
}

void Input::update(GameStateData* gsd)
{
#ifdef ARCADE

	if (m_keyboardState.Enter)
	{

	}

	no_players = 2;

#else
	//for computer input(1 player)
	m_prevKeyboardState = m_keyboardState;
	m_keyboardState = m_keyboard->GetState();

	//for controller input
	gsd->no_players = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_prevGamePadState[i] = m_gamePadState[i];
		m_gamePadState[i] = m_gamePad->GetState(i);

		if (m_gamePadState[i].IsConnected())
		{
			gsd->no_players++;
		}

		//INPUTS!
		if ((m_keyboardState.Escape && !m_prevKeyboardState.Escape)
			|| (m_gamePadState[i].IsStartPressed() && !m_prevGamePadState[i].IsStartPressed()))
		{
			inputs[i] = START;
		}
		//Up Down Left Right
		if (m_keyboardState.A && !m_keyboardState.A
			|| m_gamePadState[i].IsDPadLeftPressed()
			|| m_gamePadState[i].IsLeftThumbStickLeft())
		{
			inputs[i] = LEFT;
		}
		else if (m_keyboardState.D && !m_keyboardState.D
			|| m_gamePadState[i].IsDPadRightPressed()
			|| m_gamePadState[i].IsLeftThumbStickRight())
		{
			inputs[i] = RIGHT;
		}
		else if (m_keyboardState.W && !m_keyboardState.W
			|| m_gamePadState[i].IsDPadLeftPressed()
			|| m_gamePadState[i].IsLeftThumbStickLeft())
		{
			inputs[i] = UP;
		}
		else if (m_keyboardState.S && !m_keyboardState.S
			|| m_gamePadState[i].IsDPadRightPressed()
			|| m_gamePadState[i].IsLeftThumbStickRight())
		{
			inputs[i] = DOWN;
		}

		// Jump variations
		if ((m_keyboardState.S
			&& m_keyboardState.Space)
			|| ((m_gamePadState[i].IsAPressed()
				&& !m_prevGamePadState[i].IsAPressed())
				&& (m_gamePadState[i].IsDPadDownPressed()
					|| m_gamePadState[i].IsLeftThumbStickDown())))
		{
			inputs[i] = DOWN_A;
		}
		else if ((m_keyboardState.Space
			&& !m_prevKeyboardState.Space)
			|| (m_gamePadState[i].IsAPressed()
				&& !m_prevGamePadState[i].IsAPressed()))
		{
			inputs[i] = A;
		}
		// Bonus Jump
		if ((m_keyboardState.X
			&& m_keyboardState.W)
			|| ((m_gamePadState[i].IsXPressed()
				&& !m_prevGamePadState[i].IsXPressed())
				&& (m_gamePadState[i].IsDPadUpPressed()
					|| m_gamePadState[i].IsLeftThumbStickUp())))
		{
			inputs[i] = UP_X;
		}
		//Down Slam
		else if ((m_keyboardState.X
			&& m_keyboardState.S)
			|| ((m_gamePadState[i].IsXPressed()
				&& !m_prevGamePadState[i].IsXPressed())
				&& (m_gamePadState[i].IsDPadDownPressed()
					|| m_gamePadState[i].IsLeftThumbStickDown())))
		{
			inputs[i] = DOWN_X;
		}
		else if ((m_keyboardState.X && !m_prevKeyboardState.X)
			|| (m_gamePadState[i].IsXPressed() && !m_prevGamePadState[i].IsXPressed()))
		{
			inputs[i] = X;
		}

	}
	if (gsd->no_players <= 1)
	{
		gsd->no_players = 4;
	}
#endif
}

void Input::ResumeInput()
{
	m_gamePad->Resume();
	m_buttons.Reset();
}

void Input::SuspendInput()
{
	m_gamePad->Suspend();
}


