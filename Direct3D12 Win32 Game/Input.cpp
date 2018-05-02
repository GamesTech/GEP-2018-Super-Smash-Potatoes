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
	gsd->m_prevKeyboardState = gsd->m_keyboardState;
	gsd->m_keyboardState = m_keyboard->GetState();

	gsd->no_players = 0;
	for (int i = 0; i < gsd->MAX_PLAYERS; i++)
	{
		gsd->m_prevGamePadState[i] = gsd->m_gamePadState[i];
		gsd->m_gamePadState[i] = m_gamePad->GetState(i);
		if (gsd->m_gamePadState[i].IsConnected())
		{
			gsd->no_players++;
		}
	}
	if (gsd->no_players <= 1)
	{
		gsd->no_players = 4;
	}
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


