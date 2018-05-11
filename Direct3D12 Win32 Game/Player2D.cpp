#include "pch.h"
#include "Player2D.h"
#include "GameStateData.h"

Player2D::Player2D(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer) :Physics2D(_RD, _filename, image_buffer)
{
	//SetBoundingBoxes();
	//BottomOrigin();
	//SetMass(100);
}

Player2D::~Player2D()
{
}

void Player2D::Tick(GameStateData * _GSD, int _test, Input* input_manager/*, GameObject2D* _obj*/)
{
	if (m_vel.x > 30 || m_vel.x < -30 || m_vel.y > 500 || m_vel.y < -500)
	{
		m_ledge_jump = false;
	}
	//SetBoundingBoxes();
	if (!m_remove_controll)
	{
		controller(input_manager, _GSD);
	}
	ProcessCollision();
	AnimationChecks(_GSD);
	AddGravity(m_grounded);

	Physics2D::Tick(_GSD, m_y_coll, m_x_coll, m_new_pos, m_grabing_side);

	if (m_vel.x > m_max_speed.x && !m_remove_controll) { m_vel.x = m_max_speed.x; }
	if (m_vel.x < -m_max_speed.x && !m_remove_controll) { m_vel.x = -m_max_speed.x; }
	if (m_vel.y > 700 || m_vel.y < 0)
	{
		m_ignore_collision = false;
	}
	if (m_invincibility)
	{
		RespawnTimer(_GSD);
	}


	deathZone(_GSD);
	updateOrientation();
}

void Player2D::AnimationChecks(GameStateData * _GSD)
{
	if (m_punch_anim)
	{
		action_jump = PUNCH;
	}
	else
	{
		if (m_anim_grounded)
		{
			if (action_jump != GROUND && action_jump != PUNCH)
			{
				particle_system->addParticles(6, Particle_Type::DUST, m_pos + Vector2{ m_size.x / 2 - 20, m_size.y - 25 }, GetFlipH());
			}
			action_jump = GROUND;
			if (m_down_punching_anim)
			{
				m_execute_attack = Attack::THIRD;
			}
			m_down_punching_anim = false;
		}
		else
		{
			if (m_remove_controll)
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
					else if (m_up_punch_anim)
					{
						action_jump = UPWARDPUNCH;
						//m_attack = true;
					}
				}
				else if (m_vel.y > 300)
				{
					if (m_down_punching_anim)
					{
						action_jump = DOWNWARDPUNCH;
					}
					else
					{
						action_jump = FALL;
						m_up_punch_anim = false;
						if (m_execute_attack == Attack::SECOND)
						{
							m_execute_attack = Attack::NONE;
						}
					}
				}
			}

		}
	}
	if (!m_invincibility)
	{
		SetOpacity(1.f);
	}
	HitTimer(_GSD);
	Grabbing();
	PunchTimer(_GSD);
	UpPunchTimer(_GSD);
	AnimationTick(_GSD);
}

void Player2D::HitTimer(GameStateData * _GSD)
{
	if (m_timer_hit >= 0.4)
	{
		m_immune = false;
	}
	if (m_timer_hit >= 0.8)
	{
		m_remove_controll = false;
	}
	m_timer_hit += _GSD->m_dt;
}

void Player2D::PunchTimer(GameStateData * _GSD)
{
	if (m_timer_punch >= 0.18 && m_timer_punch <= 0.22)
	{
		if (punch_particle)
		{
			particle_system->addParticles(1, Particle_Type::ATTACK, m_pos + Vector2{ m_size.x / 3, 0 }, GetFlipH());
			punch_particle = false;
		}
	}
	else if (m_timer_punch >= 0.3)
	{
		if (m_punch_anim)
		{
			m_execute_attack = Attack::FIRST;
		}
		m_punch_anim = false;
		punch_particle = true;
	}
	m_timer_punch += _GSD->m_dt;
}

void Player2D::UpPunchTimer(GameStateData * _GSD)
{
	if (m_up_timer_punch >= 0.05)
	{
		if (m_up_punch_anim)
		{
			m_execute_attack = Attack::SECOND;
		}
		m_up_punch_anim = false;
	}
	if (m_up_timer_punch >= 0.3)
	{
		if (m_execute_attack == Attack::SECOND)
		{
			m_execute_attack = Attack::NONE;
		}
	}
	m_up_timer_punch += _GSD->m_dt;
}

void Player2D::deathZone(GameStateData * _GSD)
{
	if ((m_pos.x < -800) 
		|| (m_pos.y < -600)
		|| (m_pos.x > m_limit.x)
		|| (m_pos.y > m_limit.y))
	{
		respawn(_GSD);
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
			FlipH(false);
		}
		else
		{
			FlipH(true);
		}
		m_grabing_side = true;
		m_up_punch_anim = false;
		m_down_punching_anim = false;
		//m_grounded = true;
		action_movement = GRAB;
	}
}

void Player2D::respawn(GameStateData * _GSD)
{
	if (lives_remaining > 1)
	{
		lives_remaining--;
		action_jump = GROUND;
		m_pos = m_respawn_pos;
		m_vel.x = 0.0f;
		m_vel.y = 301.0f;
		m_damage = 1;
		m_execute_attack = Attack::NONE;
		m_invincibility = true;
		m_respawn_timer = 0;
		m_up_punch_anim = false;
		m_down_punching_anim = false;
	}
	else
	{
		if (!m_dead)
		{
			_GSD->player_podium_position[player_no] = _GSD->position_in_podium;
			_GSD->position_in_podium--;
		}
		m_pos = Vector2(-10000, 0);
		m_dead = true;
		lives_remaining = 0;
	}
	if (item != nullptr) 
	{
		item->throwItem(_GSD, m_direction);
		item = nullptr;
	}
	//audio_manager->playSound(EXPLOSION);
}

void Player2D::RespawnTimer(GameStateData * _GSD)
{
	if (m_respawn_timer >= 3)
	{
		if (m_invincibility)
		{
			m_invincibility = false;
		}
	}
	else
	{
		if ((int)(m_respawn_timer*m_respawn_timer * 2) % 2 == 0)
		{
				SetOpacity(0.3f);
		}
		else
		{
			SetOpacity(0.6f);
		}
		m_respawn_timer += _GSD->m_dt;
	}
}

void Player2D::controller(Input * input_manager, GameStateData * _GSD)
{
	// Walk
	if (input_manager->inputs[player_no] == Inputs::LEFT)
	{
		AddForce(-m_drive * Vector2::UnitX);
		FlipH(true);
		if (!m_grabing_side)
		{
			action_movement = WALK;
		}

	}
	else if (input_manager->inputs[player_no] == Inputs::RIGHT)
	{
		AddForce(m_drive * Vector2::UnitX);
		FlipH(false);
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


	//Item Throw
	if (input_manager->inputs[player_no] == Inputs::B)
	{
		if (item != nullptr) {
			item->throwItem(_GSD, m_direction);
			item = nullptr;
		}
	}


	// Jump
	if (input_manager->inputs[player_no] == Inputs::DOWN_A)
	{
		//m_vel.y = 0;
		AddForce(100 * Vector2::UnitY);
		m_ignore_collision = true;
		m_coll_state = Collision::COLNONE;
	}
	else if (input_manager->inputs[player_no] == Inputs::A)
	{
		if (m_grounded)
		{
			AddForce(-m_jumpForce * Vector2::UnitY);
			m_grounded = false;
			m_coll_state = Collision::COLNONE;
			m_up_punch_anim = false;
			m_jumping = true;
			if (m_grabing_side)
			{
				m_ledge_jump = true;
			}
		}
	}

	// Bonus Jump
	if (input_manager->inputs[player_no] == Inputs::UP_X)
	{
		if (m_bonus_jump && !m_punch_anim && !m_up_punch_anim)
		{
			m_invincibility = false;
			m_vel.y = 0;
			AddForce(-100000 * Vector2::UnitY);
			m_bonus_jump = false;
			m_coll_state = Collision::COLNONE;
			m_jumping = false;
			m_up_punch_anim = true;
			m_up_timer_punch = 0;
			particle_system->addParticles(1, Particle_Type::ATTACK_UPWARDS, m_pos + Vector2{ 0, 0 }, GetFlipH());
		}
	}
	//slam
	else if (input_manager->inputs[player_no] == Inputs::DOWN_X)
	{
		if ( !m_punch_anim && !m_up_punch_anim && !m_grounded)
		{
			m_invincibility = false;
			m_vel.y = 0;
			AddForce(m_jumpForce * Vector2::UnitY);
			m_bonus_jump = false;
			m_coll_state = Collision::COLNONE;
			m_jumping = false;
			m_down_punching_anim = true;
			//m_up_timer_punch = 0;
		}
	}
	else if (input_manager->inputs[player_no] == Inputs::X)
	{
		if (!m_punch_anim && !m_up_punch_anim && !m_grabing_side)
		{
			Punch();
			m_invincibility = false;
			m_punch_anim = true;
			m_timer_punch = 0;
		}
	}
}

void Player2D::setItem(Item * item)
{
	this->item = item;
}

bool Player2D::CheckBlocking(GameStateData * _GSD, Player2D * other_player)
{
	if (other_player->GetAttackType() != Attack::NONE
		&& GetOrientation() != other_player->GetOrientation())
	{
		other_player->Block(_GSD);
		other_player->ResetAttacks();
		return true;
	}
	return false;
}

bool Player2D::ExectuePunch(GameStateData * _GSD, Player2D * other_player)
{
	other_player->GotHit(_GSD, (m_direction/2), -1);
	return true;
}

bool Player2D::ExectueUpPunch(GameStateData * _GSD, Player2D * other_player)
{
	if (!other_player->GetImmune())
	{
		other_player->SetImmune(true);
		other_player->GotHit(_GSD, 0, -1);
		return true;
	}
	return false;
}

bool Player2D::ExectueDownPunch(GameStateData * _GSD, Player2D * other_player)
{
	float x1 = GetPos().x + (Width() / 2);
	float y1 = GetPos().y + Height();

	float x2 = other_player->GetPos().x + (other_player->Width() / 2);
	float y2 = other_player->GetPos().y + (other_player->Height() / 2);

	Vector2 direction = Vector2(x2 - x1, y2 - y1);
	direction.Normalize();

	other_player->GotHit(_GSD, direction.x, direction.y);
	return true;
}

void Player2D::GotHit(GameStateData * _GSD, float _dir, float y_force)
{
	m_grounded = false;
	m_coll_state = Collision::COLNONE;
	m_vel = Vector2::Zero;
	Vector2 impact(m_jumpForce * m_damage * _dir, m_jumpForce * m_damage * y_force);
	AddForce(impact);
	m_damage *= 1.1;
	Physics2D::Tick(_GSD, false, false, m_new_pos, m_grabing_side);
	m_remove_controll = true;
	m_timer_hit = 0;
}

void Player2D::Block(GameStateData * _GSD)
{
	m_grounded = false;
	m_coll_state = Collision::COLNONE;
	AddForce(25000 * Vector2::UnitX * -m_direction);
	Physics2D::Tick(_GSD, false, false, m_new_pos, m_grabing_side);
	m_remove_controll = true;
	m_timer_hit = 0;
}


Item * Player2D::getItem() const
{
	return item;
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

void Player2D::updateOrientation()
{
	if (!GetFlipH())
	{
		m_direction = 1;
	}
	else
	{
		m_direction = -1;
	}
}

bool Player2D::GetOrientation()
{
	return !GetFlipH();
}
