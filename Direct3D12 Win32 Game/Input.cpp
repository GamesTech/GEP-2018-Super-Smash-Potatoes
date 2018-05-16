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
	for (int i = 0; i < 4; ++i)
	{
		inputs[i] = Inputs::CLEAR;
	}
	for(int i = 0; i <= PRESSED_MAX ; i++)
	{
		pressed.push_back(false);
	}
}

void Input::update(GameStateData* gsd)
{
	prev_keyboard_state = keyboard_state;
	keyboard_state = m_keyboard->GetState();

	getPressed();

#ifdef ARCADE
	gsd->no_players = 2;

	if (pressed[X])
	{
		inputs[0] = Inputs::START;
	}

	//player 1
	//Left Right
	if (current_scene == CurrentScene::GAME)
	{
		//held check not a pressed check
		if (keyboard_state.Left)
		{
			inputs[0] = Inputs::LEFT;
		}
		else if (keyboard_state.Right)
		{
			inputs[0] = Inputs::RIGHT;
		}
	}
	else
	{
		if (pressed[LEFT])
		{
			inputs[0] = Inputs::LEFT;
		}
		else if (pressed[RIGHT])
		{
			inputs[0] = Inputs::RIGHT;
		}
	}
	//Up Down
	if (pressed[UP])
	{
		inputs[0] = Inputs::UP;
	}
	else if (pressed[DOWN])
	{
		inputs[0] = Inputs::DOWN;
	}

	// Jump variations
	// Drop Down
	if (keyboard_state.Down && keyboard_state.LeftControl)
	{
		inputs[0] = Inputs::DOWN_A;
	}
	// Bonus Jump
	else if (keyboard_state.Up && keyboard_state.LeftShift)
	{
		inputs[0] = Inputs::UP_X;
	}
	//Jump
	else if (pressed[LEFT_CTRL])
	{
		inputs[0] = Inputs::A;
	}

	//Down Slam
	else if (pressed[Z])
	{
		inputs[0] = Inputs::B;
	}
	//Punch
	else if (pressed[LEFT_SHIFT])
	{
		inputs[0] = Inputs::X;
	}
	if (pressed[LEFT_ALT])
	{
		inputs[0] = Inputs::Y;
	}

	//player2
	//Left Right
	if (current_scene == CurrentScene::GAME)
	{
		//In game needs to be smooth
		if (keyboard_state.D)
		{
			inputs[1] = Inputs::LEFT;
		}
		else if (keyboard_state.G)
		{
			inputs[1] = Inputs::RIGHT;
		}
	}
	else
	{
		//For Menus
		if (pressed[D])
		{
			inputs[1] = Inputs::LEFT;
		}
		else if (pressed[G])
		{
			inputs[1] = Inputs::RIGHT;
		}
	}
	//Up Down
	if (pressed[R])
	{
		inputs[1] = Inputs::UP;
	}
	else if (pressed[F])
	{
		inputs[1] = Inputs::DOWN;
	}

	// Jump variations
	// Drop Down
	else if (keyboard_state.F && keyboard_state.A)
	{
		inputs[1] = Inputs::DOWN_A;
	}
	// Bonus Jump
	else if (keyboard_state.R && keyboard_state.W)
	{
		inputs[1] = Inputs::UP_X;
	}
	//Jump
	else if (pressed[A])
	{
		inputs[1] = Inputs::A;
	}
	//Down Slam
	else if (pressed[I])
	{
		inputs[1] = Inputs::B;
	}
	//Punch
	else if (pressed[W])
	{
		inputs[1] = Inputs::X;
	}
	//Throw
	if (pressed[S])
	{
		inputs[1] = Inputs::Y;
	}

#else
	//for controller input
	gsd->no_players = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		prev_GP_state[i] = GP_state[i];
		GP_state[i] = m_gamePad->GetState(i);

		if (GP_state[i].IsConnected())
		{
			gsd->no_players++;
		}

		//INPUTS!
		if ((pressed[ESCAPE])
			|| (GP_state[i].IsStartPressed() && !prev_GP_state[i].IsStartPressed()))
		{
			inputs[i] = Inputs::START;
		}

		if ((pressed[Q])
			|| (GP_state[0].IsLeftShoulderPressed() && !prev_GP_state[0].IsLeftShoulderPressed()))
		{
			inputs[i] = Inputs::LB;
		}
		if ((pressed[E])
			|| (GP_state[0].IsRightShoulderPressed() && !prev_GP_state[0].IsRightShoulderPressed()))
		{
			inputs[i] = Inputs::RB;
		}

		//Up Down Left Right
		if (current_scene == CurrentScene::GAME)
		{
			if ((keyboard_state.A)
				|| (GP_state[i].IsDPadLeftPressed() && !prev_GP_state[i].IsDPadLeftPressed())
				|| GP_state[i].IsLeftThumbStickLeft())
			{
				inputs[i] = Inputs::LEFT;
			}
			else if ((keyboard_state.D)
				|| (GP_state[i].IsDPadRightPressed() && !prev_GP_state[i].IsDPadRightPressed())
				|| GP_state[i].IsLeftThumbStickRight())
			{
				inputs[i] = Inputs::RIGHT;
			}
		}
		else
		{
			if ((pressed[A])
				|| (GP_state[i].IsDPadLeftPressed() && !prev_GP_state[i].IsDPadLeftPressed())
				|| GP_state[i].IsLeftThumbStickLeft())
			{
				inputs[i] = Inputs::LEFT;
			}
			else if ((pressed[D])
				|| (GP_state[i].IsDPadRightPressed() && !prev_GP_state[i].IsDPadRightPressed())
				|| GP_state[i].IsLeftThumbStickRight())
			{
				inputs[i] = Inputs::RIGHT;
			}
		}
		if ((pressed[W])
			|| (GP_state[i].IsDPadUpPressed() && !prev_GP_state[i].IsDPadUpPressed())
			|| GP_state[i].IsLeftThumbStickUp())
		{
			inputs[i] = Inputs::UP;
		}
		else if ((pressed[S])
			|| (GP_state[i].IsDPadDownPressed() && !prev_GP_state[i].IsDPadDownPressed())
			|| GP_state[i].IsLeftThumbStickDown())
		{
			inputs[i] = Inputs::DOWN;
		}

		// Jump variations
		if ((keyboard_state.S && keyboard_state.Space)
			|| ((GP_state[i].IsAPressed() && !prev_GP_state[i].IsAPressed())
				&& (GP_state[i].IsDPadDownPressed() || GP_state[i].IsLeftThumbStickDown())))
		{
			inputs[i] = Inputs::DOWN_A;
		}
		else if ((pressed[SPACE])
			|| (GP_state[i].IsAPressed() && !prev_GP_state[i].IsAPressed()))
		{
			inputs[i] = Inputs::A;
		}
		// Bonus Jump
		if ((keyboard_state.X && keyboard_state.W)
			|| ((GP_state[i].IsXPressed() && !prev_GP_state[i].IsXPressed())
				&& (GP_state[i].IsDPadUpPressed() || GP_state[i].IsLeftThumbStickUp())))
		{
			inputs[i] = Inputs::UP_X;
		}
		//Down Slam
		else if ((keyboard_state.X && keyboard_state.S)
			|| ((GP_state[i].IsXPressed() && !prev_GP_state[i].IsXPressed())
				&& (GP_state[i].IsDPadDownPressed() || GP_state[i].IsLeftThumbStickDown())))
		{
			inputs[i] = Inputs::DOWN_X;
		}
		else if ((pressed[X])
			|| (GP_state[i].IsXPressed() && !prev_GP_state[i].IsXPressed()))
		{
			inputs[i] = Inputs::X;
		}
		else if ((pressed[BACK])
			|| (GP_state[i].IsBPressed() && !prev_GP_state[i].IsBPressed()))
		{
			inputs[i] = Inputs::B;
		}
		else if ((pressed[R])
			|| (GP_state[i].IsYPressed() && !prev_GP_state[i].IsYPressed()))
		{
			inputs[i] = Inputs::Y;
		}
		if (current_scene != CurrentScene::GAME)
		{
			//keyboard only (for debug)
			if (pressed[ESCAPE])
			{
				inputs[i] = Inputs::ESCAPE;
			}
			if (pressed[ENTER])
			{
				inputs[i] = Inputs::ENTER;
			}
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
	std::fill(pressed.begin(), pressed.end(), false);
	//for (int i = 0; i <= PRESSED_MAX; i++)
	//{
	//	pressed[i] = false;
	//}

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

void Input::getPressed()
{
	if (keyboard_state.W && !prev_keyboard_state.W)
	{
		pressed[W] = true;
	}
	if (keyboard_state.A && !prev_keyboard_state.A)
	{
		pressed[A] = true;
	}
	if (keyboard_state.S && !prev_keyboard_state.S)
	{
		pressed[S] = true;
	}
	if (keyboard_state.D && !prev_keyboard_state.D)
	{
		pressed[D] = true;
	}

	if (keyboard_state.Up && !prev_keyboard_state.Up)
	{
		pressed[UP] = true;
	}
	if (keyboard_state.Down && !prev_keyboard_state.Down)
	{
		pressed[DOWN] = true;
	}
	if (keyboard_state.Left && !prev_keyboard_state.Left)
	{
		pressed[LEFT] = true;
	}
	if (keyboard_state.Right && !prev_keyboard_state.Right)
	{
		pressed[RIGHT] = true;
	}

	if (keyboard_state.LeftControl && !prev_keyboard_state.LeftControl)
	{
		pressed[LEFT_CTRL] = true;
	}
	if (keyboard_state.LeftShift && !prev_keyboard_state.LeftShift)
	{
		pressed[LEFT_SHIFT] = true;
	}
	if (keyboard_state.LeftAlt && !prev_keyboard_state.LeftAlt)
	{
		pressed[LEFT_ALT] = true;
	}

	if (keyboard_state.Q && !prev_keyboard_state.Q)
	{
		pressed[Q] = true;
	}
	if (keyboard_state.E && !prev_keyboard_state.E)
	{
		pressed[E] = true;
	}

	if (keyboard_state.X && !prev_keyboard_state.X)
	{
		pressed[X] = true;
	}
	if (keyboard_state.Z && !prev_keyboard_state.Z)
	{
		pressed[Z] = true;
	}
	if (keyboard_state.R && !prev_keyboard_state.R)
	{
		pressed[R] = true;
	}
	if (keyboard_state.F && !prev_keyboard_state.F)
	{
		pressed[F] = true;
	}
	if (keyboard_state.G && !prev_keyboard_state.G)
	{
		pressed[G] = true;
	}
	if (keyboard_state.I && !prev_keyboard_state.I)
	{
		pressed[I] = true;
	}

	if (keyboard_state.Space && !prev_keyboard_state.Space)
	{
		pressed[SPACE] = true;
	}
	if (keyboard_state.Enter && !prev_keyboard_state.Enter)
	{
		pressed[ENTER] = true;
	}
	if (keyboard_state.Escape && !prev_keyboard_state.Escape)
	{
		pressed[ESCAPE] = true;
	}
	if (keyboard_state.Back && !prev_keyboard_state.Back)
	{
		pressed[BACK] = true;
	}
}
