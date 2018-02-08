#include "pch.h"
#include "Player2D.h"
#include "GameStateData.h"

Player2D::Player2D(RenderData* _RD, string _filename):Physics2D(_RD,_filename)
{
	CentreOrigin();
}


Player2D::~Player2D()
{
}

void Player2D::Tick(GameStateData * _GSD)
{

//Push the guy around in the directions for the key presses
	if (_GSD->m_keyboardState.W || _GSD->m_gamePadState.IsDPadUpPressed() || _GSD->m_gamePadState.IsLeftThumbStickUp())
	{
		AddForce(-m_drive * Vector2::UnitY);
	}
	if (_GSD->m_keyboardState.S || _GSD->m_gamePadState.IsDPadDownPressed() || _GSD->m_gamePadState.IsLeftThumbStickDown())
	{
		AddForce(m_drive * Vector2::UnitY);
	}
	if (_GSD->m_keyboardState.A || _GSD->m_gamePadState.IsDPadLeftPressed() || _GSD->m_gamePadState.IsLeftThumbStickLeft())
	{
		AddForce(-m_drive * Vector2::UnitX);
	}
	if (_GSD->m_keyboardState.D || _GSD->m_gamePadState.IsDPadRightPressed() || _GSD->m_gamePadState.IsLeftThumbStickRight())
	{
		AddForce(m_drive * Vector2::UnitX);
	}

	if (_GSD->m_keyboardState.Space || _GSD->m_gamePadState.IsAPressed())
	{
		AddForce(-(m_drive * 10) * Vector2::UnitY);
	}


	Vector2 mousePush = Vector2(_GSD->m_mouseState.x, _GSD->m_mouseState.y);
	
	//AddForce(m_drive*mousePush);
	
//GEP:: Lets go up the inheritence and share our functionality

	Physics2D::Tick(_GSD);

//after that as updated my position let's lock it inside my limits
	if (m_pos.x < 0.0f)
	{
		m_pos.x *= -1.0f;
		m_vel.x *= -1.0f; // yea, not a nice bounce for works okay for a first pass
	}
	if (m_pos.y < 0.0f)
	{
		m_pos.y *= -1.0f;
		m_vel.y *= -1.0f;
	}

	if (m_pos.x > m_limit.x)
	{
		m_pos.x = 2.0f * m_limit.x - m_pos.x;
		m_vel.x *= -1.0f;
	}
	if (m_pos.y > m_limit.y)
	{
		m_pos.y = 2.0f * m_limit.y - m_pos.y;
		m_vel.y *= -1.0f;
	}
}
