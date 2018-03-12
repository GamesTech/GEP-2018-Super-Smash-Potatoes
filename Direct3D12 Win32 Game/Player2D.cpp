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
	//if (m_coll_state == Collision::COLTOP || m_coll_state == Collision::COLBOTTOM)
	//{
	//	m_y_coll = true;
	//}
	//else
	//{
	//	m_y_coll = false;
	//}
	//if (m_coll_state == Collision::COLLEFT || m_coll_state == Collision::COLRIGHT)
	//{
	//	m_x_coll = true;
	//}
	//else
	//{
	//	m_x_coll = false;
	//}

	if (m_vel.x > 30 || m_vel.x < -30 || m_vel.y > 500 || m_vel.y < -500)
	{
		m_ledge_jump = false;
	}



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
	}
	Grabbing();
	AnimationTick(_GSD);

	AddGravity(m_grounded);
	//GEP:: Lets go up the inheritence and share our functionality
	Physics2D::Tick(_GSD, m_y_coll, m_x_coll, m_new_pos, m_grabing_side);

	if (m_vel.x > m_max_speed.x) { m_vel.x = m_max_speed.x; }
	if (m_vel.x < -m_max_speed.x) { m_vel.x = -m_max_speed.x; }

	//after that as updated my position let's lock it inside my limits
	deathZone();
}

void Player2D::deathZone()
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

void Player2D::setPlayerNo(int player_number)
{
	player_no = player_number;
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
			//if (m_grounded)
			//{
			//	AddForce(-10000.f * Vector2::UnitX);
			//}
		}
		else
		{
			direction = RIGHT;
			//if (m_grounded )
			//{
			//	AddForce(10000.f * Vector2::UnitX);
			//}
		}
		m_grabing_side = true;
		//m_grounded = true;
		action_movement = GRAB;
	}
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
			m_grounded = false;
			m_coll_state = Collision::COLNONE;
			if (m_grabing_side)
			{
				m_ledge_jump = true;
			}
		}
	}

	if ((_GSD->m_keyboardState.X && _GSD->m_keyboardState.W) || ((_GSD->m_gamePadState[player_no].IsXPressed() && !_GSD->m_prevGamePadState[player_no].IsXPressed()) && (_GSD->m_gamePadState[player_no].IsDPadUpPressed() || _GSD->m_gamePadState[player_no].IsLeftThumbStickUp())))
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
		m_grounded = true;
		m_bonus_jump = true;
		m_y_coll = true;
		m_x_coll = false;
		//m_pos.y = m_new_pos;
		//m_vel.y = 0;
		break;
	case COLBOTTOM:
		m_y_coll = true;
		m_x_coll = false;
		//m_grounded = true;
		//m_pos.y = m_new_pos;
		//m_vel.y = 0;
		break;
	case COLRIGHT:
		m_grounded = true;
		m_x_coll = true;
		m_y_coll = false;
		//m_bonus_jump = true;
		//m_pos.x = m_new_pos;
		//m_vel.x = 0;
		break;
	case COLLEFT:
		m_grounded = true;
		m_x_coll = true;
		m_y_coll = false;
		//m_bonus_jump = true;
		//m_pos.x = m_new_pos;
		//m_vel.x = 0;
		break;
	case COLNONE:
		m_x_coll = false;
		m_y_coll = false;
		m_grounded = false;
		//m_ledge_jump = false;
		break;
	}
}

