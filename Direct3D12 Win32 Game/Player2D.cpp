#include "pch.h"
#include "Player2D.h"
#include "GameStateData.h"

Player2D::Player2D(RenderData* _RD, string _filename) :Physics2D(_RD, _filename)
{
	SetBoundingBoxes();
	//BottomOrigin();
	//SetMass(100);
}


Player2D::~Player2D()
{
}

void Player2D::Tick(GameStateData * _GSD/*, GameObject2D* _obj*/)
{
	//Push the guy around in the directions for the key presses
	SetBoundingBoxes();
	controller(_GSD);
	
	ProcessCollision();

	if (m_anim_grounded)
	{
		action_jump = GROUND;
	}
	else
	{
		if (m_vel.y < 300)
		{
			action_jump = JUMP;
		}
		else if (m_vel.y > 300)
		{
			action_jump = FALL;
		}
		m_grounded = false;
	}

	//GEP:: Lets go up the inheritence and share our functionality
	//after that as updated my position let's lock it inside my limits
	if (m_vel.x > m_max_speed.x) { m_vel.x = m_max_speed.x; }
	if (m_vel.x < -m_max_speed.x) { m_vel.x = -m_max_speed.x; }
	AddGravity(m_grounded);
	Physics2D::Tick(_GSD);
	RespawnPlayer();
	if (m_jumping)
	{
		if (jumping_timer >= 1)
		{
			m_jumping = false;
		}
		else
		{
			jumping_timer += _GSD->m_dt;
		}
	}
	else
	{
		jumping_timer = 0;
	}
}

void Player2D::RespawnPlayer()
{
	if (m_pos.x < 0.0f - 500)
	{
		respawn();
	}
	if (m_pos.y < 0.0f - 200)
	{
		respawn();
	}

	if (m_pos.x > m_limit.x + 500)
	{
		respawn();
	}
	if (m_pos.y > m_limit.y + 200)
	{
		respawn();
	}
}

void Player2D::Grabbing()
{
	if (m_coll_state != COLRIGHT && m_coll_state != COLLEFT)
	{

		m_grabing_side = false;
	}
	else
	{
		if (m_coll_state == COLRIGHT)
		{
			direction = LEFT;
		}
		else
		{
			direction = RIGHT;
		}
		m_grabing_side = true;
		action_movement = GRAB;
		m_vel.y = 0;
	}
}

void Player2D::setPlayerNo(int player_number)
{
	player_no = player_number;
}

void Player2D::respawn()
{
	m_pos.x = 400.0f;
	m_pos.y = 300.0f;
	m_vel.x = 0.0f;
	m_vel.y = 301.0f;
}

void Player2D::controller(GameStateData * _GSD)
{
	if (_GSD->m_keyboardState.A || _GSD->m_gamePadState[player_no].IsDPadLeftPressed() || _GSD->m_gamePadState[player_no].IsLeftThumbStickLeft())
	{
		AddForce(-m_drive * Vector2::UnitX);
		direction = LEFT;
		if (!m_grabing_side)
		{
			action_movement = WALK;
		}

	}
	else if (_GSD->m_keyboardState.D || _GSD->m_gamePadState[player_no].IsDPadRightPressed() || _GSD->m_gamePadState[player_no].IsLeftThumbStickRight())
	{
		AddForce(m_drive * Vector2::UnitX);
		direction = RIGHT;
		if (!m_grabing_side)
		{
			action_movement = WALK;
		}
	}
	else
	{
		if (!m_grabing_side)
		{
			action_movement = STILL;
		}
	}

	if ((_GSD->m_keyboardState.Space && !_GSD->m_prevKeyboardState.Space) || (_GSD->m_gamePadState[player_no].IsAPressed() && !_GSD->m_prevGamePadState[player_no].IsAPressed()))
	{
		if (m_grounded)
		{
			AddForce(-m_jumpForce * Vector2::UnitY);
			m_jumping = true;
			m_grounded = false;
		}
	}

	if ((_GSD->m_keyboardState.X && _GSD->m_keyboardState.Up) || ((_GSD->m_gamePadState[player_no].IsXPressed() && !_GSD->m_prevGamePadState[player_no].IsXPressed()) && (_GSD->m_gamePadState[player_no].IsDPadUpPressed() || _GSD->m_gamePadState[player_no].IsLeftThumbStickUp())))
	{
		if (m_bonus_jump)
		{
			m_vel.y = 0;
			AddForce(-m_jumpForce * Vector2::UnitY);
			m_bonus_jump = false;
		}
	}
}

void Player2D::ProcessCollision()
{
	switch (m_coll_state)
	{
	case COLTOP:
		//m_vel.y = 0;
		m_grounded = true;
		m_bonus_jump = true;
		m_pos.y = m_new_pos;
		break;
	case COLBOTTOM:
		m_grounded = true;
		m_pos.y = m_new_pos;
		break;
	case COLRIGHT:
		m_grounded = true;
		m_bonus_jump = true;
		m_vel.x = 0;
		m_pos.x = m_new_pos;
		break;
	case COLLEFT:
		m_grounded = true;
		m_bonus_jump = true;
		//m_vel.x = 0;
		m_pos.x = m_new_pos;
		break;
	case COLNONE:
		//m_grounded = false;
		break;
	}
}
