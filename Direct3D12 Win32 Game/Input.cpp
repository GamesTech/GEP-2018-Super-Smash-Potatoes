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
	m_prevKeyboardState = m_keyboardState;
	m_keyboardState = m_keyboard->GetState();

	gsd->no_players = 2;

	if (m_keyboardState.X && !m_prevKeyboardState.X)
	{
		inputs[0] = Inputs::START;
	}
	//player 1
	if (m_keyboardState.Left /*&& !m_prevKeyboardState.A*/)
	{
		inputs[0] = Inputs::LEFT;
	}
	else if (m_keyboardState.Right /*&& !m_prevKeyboardState.A*/)
	{
		inputs[0] = Inputs::RIGHT;
	}
	else if (m_keyboardState.Up && !m_prevKeyboardState.Up)
	{
		inputs[0] = Inputs::UP;
	}
	else if (m_keyboardState.Down && !m_prevKeyboardState.Down)
	{
		inputs[0] = Inputs::DOWN;
	}

	// Jump variations
	if (m_keyboardState.Down && m_keyboardState.LeftControl)
	{
		inputs[0] = Inputs::DOWN_A;
	}
	else if (m_keyboardState.LeftControl && !m_prevKeyboardState.LeftControl)
	{
		inputs[0] = Inputs::A;
	}
	// Bonus Jump
	if (m_keyboardState.Up && m_keyboardState.LeftShift)
	{
		inputs[0] = Inputs::UP_X;
	}
	//Down Slam
	else if (m_keyboardState.LeftShift && m_keyboardState.Down)
	{
		inputs[0] = Inputs::DOWN_X;
	}
	else if (m_keyboardState.LeftShift && !m_prevKeyboardState.LeftShift)
	{
		inputs[0] = Inputs::X;
	}


	//player2
	if (m_keyboardState.D /*&& !m_prevKeyboardState.A*/)
	{
		inputs[1] = Inputs::LEFT;
	}
	else if (m_keyboardState.G /*&& !m_prevKeyboardState.A*/)
	{
		inputs[1] = Inputs::RIGHT;
	}
	else if (m_keyboardState.R && !m_prevKeyboardState.R)
	{
		inputs[1] = Inputs::UP;
	}
	else if (m_keyboardState.F && !m_prevKeyboardState.F)
	{
		inputs[1] = Inputs::DOWN;
	}
	// Jump variations
	if (m_keyboardState.F && m_keyboardState.A)
	{
		inputs[1] = Inputs::DOWN_A;
	}
	else if (m_keyboardState.A && !m_prevKeyboardState.A)
	{
		inputs[1] = Inputs::A;
	}
	// Bonus Jump
	if (m_keyboardState.R && m_keyboardState.W)
	{
		inputs[1] = Inputs::UP_X;
	}
	//Down Slam
	else if (m_keyboardState.F && m_keyboardState.W)
	{
		inputs[1] = Inputs::DOWN_X;
	}
	else if (m_keyboardState.W && !m_prevKeyboardState.W)
	{
		inputs[1] = Inputs::X;
	}

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
			inputs[i] = Inputs::START;
		}

		if ((m_keyboardState.Q && !m_prevKeyboardState.Q)
			|| (m_gamePadState[0].IsLeftShoulderPressed() && !m_prevGamePadState[0].IsLeftShoulderPressed()))
		{
			inputs[i] = Inputs::LB;
		}
		if ((m_keyboardState.E && !m_prevKeyboardState.E)
			|| (m_gamePadState[0].IsRightShoulderPressed() && !m_prevGamePadState[0].IsRightShoulderPressed()))
		{
			inputs[i] = Inputs::RB;
		}

		//Up Down Left Right
		if ((m_keyboardState.A /*&& !m_prevKeyboardState.A*/)
			|| (m_gamePadState[i].IsDPadLeftPressed() && !m_prevGamePadState[i].IsDPadLeftPressed())
			|| m_gamePadState[i].IsLeftThumbStickLeft())
		{
			inputs[i] = Inputs::LEFT;
		}
		else if ((m_keyboardState.D /*&& !m_prevKeyboardState.D*/)
			|| (m_gamePadState[i].IsDPadRightPressed() && !m_prevGamePadState[i].IsDPadRightPressed())
			|| m_gamePadState[i].IsLeftThumbStickRight())
		{
			inputs[i] = Inputs::RIGHT;
		}
		else if ((m_keyboardState.W && !m_prevKeyboardState.W)
			|| (m_gamePadState[i].IsDPadUpPressed() && !m_prevGamePadState[i].IsDPadUpPressed())
			|| m_gamePadState[i].IsLeftThumbStickUp())
		{
			inputs[i] = Inputs::UP;
		}
		else if ((m_keyboardState.S && !m_prevKeyboardState.S)
			|| (m_gamePadState[i].IsDPadDownPressed() && !m_prevGamePadState[i].IsDPadDownPressed())
			|| m_gamePadState[i].IsLeftThumbStickDown())
		{
			inputs[i] = Inputs::DOWN;
		}

		// Jump variations
		if ((m_keyboardState.S
			&& m_keyboardState.Space)
			|| ((m_gamePadState[i].IsAPressed()
				&& !m_prevGamePadState[i].IsAPressed())
				&& (m_gamePadState[i].IsDPadDownPressed()
					|| m_gamePadState[i].IsLeftThumbStickDown())))
		{
			inputs[i] = Inputs::DOWN_A;
		}
		else if ((m_keyboardState.Space
			&& !m_prevKeyboardState.Space)
			|| (m_gamePadState[i].IsAPressed()
				&& !m_prevGamePadState[i].IsAPressed()))
		{
			inputs[i] = Inputs::A;
		}
		// Bonus Jump
		if ((m_keyboardState.X
			&& m_keyboardState.W)
			|| ((m_gamePadState[i].IsXPressed()
				&& !m_prevGamePadState[i].IsXPressed())
				&& (m_gamePadState[i].IsDPadUpPressed()
					|| m_gamePadState[i].IsLeftThumbStickUp())))
		{
			inputs[i] = Inputs::UP_X;
		}
		//Down Slam
		else if ((m_keyboardState.X 
			&& !m_prevKeyboardState.X
			&& m_keyboardState.S
			&& !m_prevKeyboardState.S)
			|| ((m_gamePadState[i].IsXPressed()
				&& !m_prevGamePadState[i].IsXPressed())
				&& (m_gamePadState[i].IsDPadDownPressed()
					|| m_gamePadState[i].IsLeftThumbStickDown())))
		{
			inputs[i] = Inputs::DOWN_X;
		}
		else if ((m_keyboardState.X && !m_prevKeyboardState.X)
			|| (m_gamePadState[i].IsXPressed() && !m_prevGamePadState[i].IsXPressed()))
		{
			inputs[i] = Inputs::X;
		}
		else if ((m_keyboardState.Back && !m_prevKeyboardState.Back)
			|| (m_gamePadState[0].IsBPressed() && !m_prevGamePadState[0].IsBPressed()))
		{
			inputs[i] = Inputs::B;
		}
		else if ((m_keyboardState.R && !m_prevKeyboardState.R)
			|| (m_gamePadState[0].IsYPressed() && !m_prevGamePadState[0].IsYPressed()))
		{
			inputs[i] = Inputs::Y;
		}

	}
	if (gsd->no_players <= 1)
	{
		gsd->no_players = 4;
	}
#endif
}

bool Input::clearInput()
{
	bool temp[4] = { false, false, false, false };
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (inputs[i] != Inputs::CLEAR)
		{
			inputs[i] = Inputs::CLEAR;
			temp[i] = true;
		}
		else
		{
			temp[i] = false;
		}
	}
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (temp[i])
		{
			return true;
		}
	}
	return false;
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