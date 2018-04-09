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

//void Player2D::init(AudioManager * am)
//{
//	audio_manager = am;
//}

void Player2D::Tick(GameStateData * _GSD, int _test/*, GameObject2D* _obj*/)
{
	if (m_vel.x > 30 || m_vel.x < -30 || m_vel.y > 500 || m_vel.y < -500)
	{
		m_ledge_jump = false;
	}
	SetBoundingBoxes();
	if (!m_hit)
	{
		controller(_GSD);
	}
	ProcessCollision();
	AnimationChecks(_GSD);
	AddGravity(m_grounded);
	//GEP:: Lets go up the inheritence and share our functionality
	Physics2D::Tick(_GSD, m_y_coll, m_x_coll, m_new_pos, m_grabing_side);

	if (m_vel.x > m_max_speed.x) { m_vel.x = m_max_speed.x; }
	if (m_vel.x < -m_max_speed.x) { m_vel.x = -m_max_speed.x; }

	//after that as updated my position let's lock it inside my limits
	deathZone();
}

void Player2D::AnimationChecks(GameStateData * _GSD)
{
	if (m_punch)
	{
		action_jump = PUNCH;
	}
	else
	{
		if (m_anim_grounded)
		{
			action_jump = GROUND;
		}
		else
		{
			if (m_hit)
			{
				action_jump = HIT;
			}
			else
			{
				if (m_vel.y < 300)
				{
					if (m_jumping)
					{
						action_jump = JUMP;
					}
					else if (m_upwards_punch)
					{
						action_jump = UPWARDPUNCH;
						m_attack = true;
					}
				}
				else if (m_vel.y > 300)
				{
					action_jump = FALL;
					m_upwards_punch = false;
				}
			}

		}
	}

	HitTimer(_GSD);
	Grabbing();
	PunchTimer(_GSD);
	AnimationTick(_GSD);
}

void Player2D::HitTimer(GameStateData * _GSD)
{
	if (m_timer_hit >= 0.8)
	{
		m_hit = false;
	}
	else
	{
		m_timer_hit += _GSD->m_dt;
	}
}

void Player2D::PunchTimer(GameStateData * _GSD)
{
	if (m_timer_punch >= 0.3)
	{
		if (m_punch)
		{
			m_attack = true;
		}
		m_punch = false;
	}
	else
	{
		m_timer_punch += _GSD->m_dt;
	}
}

void Player2D::deathZone()
{
	if (m_pos.x < 0.0f - 500)
	{
		respawn();
	}
	if (m_pos.y < 0.0f - 400)
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
		}
		else
		{
			direction = RIGHT;
		}
		m_grabing_side = true;
		m_upwards_punch = false;
		//m_grounded = true;
		action_movement = GRAB;
	}
}

void Player2D::respawn()
{
	if (lives_remaining > 0)
	{
		lives_remaining--;
		action_jump = GROUND;
		m_pos.x = 400.0f;
		m_pos.y = 300.0f;
		m_vel.x = 0.0f;
		m_vel.y = 301.0f;
		m_damage = 1;
		m_upwards_punch = false;
	}
	else
	{
		m_dead = true;
	}
	//audio_manager->playSound(EXPLOSION);
}

void Player2D::controller(GameStateData * _GSD)
{
	// Walk
	if (_GSD->m_keyboardState.A 
		|| _GSD->m_gamePadState[player_no].IsDPadLeftPressed() 
		|| _GSD->m_gamePadState[player_no].IsLeftThumbStickLeft())
	{
		AddForce(-m_drive * Vector2::UnitX);
		direction = LEFT;
		if (!m_grabing_side)
		{
			action_movement = WALK;
		}

	}
	else if (_GSD->m_keyboardState.D 
		|| _GSD->m_gamePadState[player_no].IsDPadRightPressed() 
		|| _GSD->m_gamePadState[player_no].IsLeftThumbStickRight())
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


	// Jump
	if ((_GSD->m_keyboardState.Space 
		&& !_GSD->m_prevKeyboardState.Space) 
		|| (_GSD->m_gamePadState[player_no].IsAPressed() 
			&& !_GSD->m_prevGamePadState[player_no].IsAPressed()))
	{
		if (m_grounded)
		{
			AddForce(-m_jumpForce * Vector2::UnitY);
			m_grounded = false;
			m_coll_state = Collision::COLNONE;
			m_upwards_punch = false;
			m_jumping = true;
			if (m_grabing_side)
			{
				m_ledge_jump = true;
			}
		}
	}

	// Bonus Jump
	if ((_GSD->m_keyboardState.X 
		&& _GSD->m_keyboardState.W) 
		|| ((_GSD->m_gamePadState[player_no].IsXPressed() 
			&& !_GSD->m_prevGamePadState[player_no].IsXPressed()) 
			&& (_GSD->m_gamePadState[player_no].IsDPadUpPressed() 
				|| _GSD->m_gamePadState[player_no].IsLeftThumbStickUp())))
	{
		if (m_bonus_jump)
		{
			m_vel.y = 0;
			AddForce(-m_jumpForce * Vector2::UnitY);
			m_bonus_jump = false;
			m_coll_state = Collision::COLNONE;
			m_jumping = false;
			m_upwards_punch = true;
		}
	}
	else if ((_GSD->m_keyboardState.X && !_GSD->m_prevKeyboardState.X) 
		|| (_GSD->m_gamePadState[player_no].IsXPressed() && !_GSD->m_prevGamePadState[player_no].IsXPressed()))
	{
		if (!m_punch && !m_upwards_punch && !m_grabing_side)
		{
			Punch();
			m_punch = true;
			m_timer_punch = 0;
		}
	}

	//if (_GSD->m_keyboardState.Enter
	//	&& !_GSD->m_prevKeyboardState.Enter)
	//{
	//	m_attack = true;
	//}
}

void Player2D::Hit(GameStateData * _GSD, int _dir)
{
	m_grounded = false;
	m_coll_state = Collision::COLNONE;
	AddForce(-m_jumpForce * Vector2::UnitY * m_damage);
	AddForce(m_jumpForce * Vector2::UnitX * m_damage * _dir);
	m_damage *= 1.1;
	Physics2D::Tick(_GSD, false, false, m_new_pos, m_grabing_side);
	m_hit = true;
	m_timer_hit = 0;
	//audio_manager->playSound(SLAPSOUND);
}

void Player2D::UpHit(GameStateData * _GSD)
{
	m_grounded = false;
	m_coll_state = Collision::COLNONE;
	AddForce(-m_jumpForce * Vector2::UnitY * m_damage * 1.1);
	m_damage *= 1.1;
	Physics2D::Tick(_GSD, false, false, m_new_pos, m_grabing_side);
	m_hit = true;
	m_timer_hit = 0;
	//audio_manager->playSound(SLAPSOUND);
}

void Player2D::Block(GameStateData * _GSD, int _dir)
{
	m_grounded = false;
	m_coll_state = Collision::COLNONE;
	AddForce(25000 * Vector2::UnitX * _dir);
	//m_damage *= 1.1;
	Physics2D::Tick(_GSD, false, false, m_new_pos, m_grabing_side);
	m_hit = true;
	m_timer_hit = 0;
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

bool Player2D::GetOrientation()
{
	if (direction == RIGHT)
	{
		return true;
	}
	else
	{
		return false;
	}
}
