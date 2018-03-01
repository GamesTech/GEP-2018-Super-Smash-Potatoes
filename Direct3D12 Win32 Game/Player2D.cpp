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

void Player2D::Tick(GameStateData * _GSD, GameObject2D* _obj)
{
	//Physics2D::Tick(_GSD);
	//Push the guy around in the directions for the key presses
	SetBoundingBoxes();
	CheckCollision(_obj);
	controller(_GSD);

	if (!m_grounded)
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
	else
	{
		action_jump = GROUND;
	}

	AnimationTick(_GSD);
	AddGravity(m_grounded);

	Vector2 mousePush = Vector2(_GSD->m_mouseState.x, _GSD->m_mouseState.y);
	
	//AddForce(m_drive*mousePush);

	//GEP:: Lets go up the inheritence and share our functionality
	Physics2D::Tick(_GSD);

	if (m_vel.x > m_max_speed.x) { m_vel.x = m_max_speed.x; }
	if (m_vel.x < -m_max_speed.x) { m_vel.x = -m_max_speed.x; }

	//after that as updated my position let's lock it inside my limits
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

void Player2D::respawn()
{
	m_pos.x = 400.0f;
	m_pos.y = 300.0f;
	m_vel.x = 0.0f;
	m_vel.y = 301.0f;
}

void Player2D::controller(GameStateData * _GSD)
{
	if (_GSD->m_keyboardState.A || _GSD->m_gamePadState.IsDPadLeftPressed() || _GSD->m_gamePadState.IsLeftThumbStickLeft())
	{
		AddForce(-m_drive * Vector2::UnitX);
		direction = LEFT;
		action_movement = WALK;

	}
	else if (_GSD->m_keyboardState.D || _GSD->m_gamePadState.IsDPadRightPressed() || _GSD->m_gamePadState.IsLeftThumbStickRight())
	{
		AddForce(m_drive * Vector2::UnitX);
		direction = RIGHT;
		action_movement = WALK;
	}
	else
	{
		action_movement = STILL;
	}

	if ((_GSD->m_keyboardState.Space && !_GSD->m_prevKeyboardState.Space) || _GSD->m_gamePadState.IsAPressed())
	{
		if (m_grounded)
		{
			AddForce(-m_jumpForce * Vector2::UnitY);

			m_grounded = false;
		}
	}

	if ((_GSD->m_keyboardState.X && _GSD->m_keyboardState.Up))
	{
		if (m_bonus_jump)
		{
			AddForce(-m_jumpForce * Vector2::UnitY);
			m_bonus_jump = false;
		}
	}
}

void Player2D::CheckCollision(GameObject2D *_obj)
{
	GameObject2D* object = _obj;

	float width = 0.5 * (Width() + object->Width());
	float height = 0.5 * (Height() + object->Height());
	float distance_x = CenterX() - object->CenterX();
	float distance_y = CenterY() - object->CenterY();

	if (abs(distance_x) <= width && abs(distance_y) <= height)
	{
		// collision occured

		float collision_width = width * distance_y;
		float collision_heihgt = height * distance_x;

		if (collision_width > collision_heihgt)
		{
			if (collision_width > -collision_heihgt)
			{
				float newPosY = object->GetPos().y + object->Height();
				m_pos.y = newPosY;
				m_vel.y = 0.0f;
				//m_grounded = true;

				// collision at the bottom 
			}
			else
			{
				float newPosX = object->GetPos().x - m_size.x;
				m_pos.x = newPosX;
				m_vel.x = 0.0f;
				m_grounded = true;
				m_bonus_jump = true;
				// on the left 
			}
		}
		else
		{
			if (collision_width > -collision_heihgt)
			{
				float newPosX = object->GetPos().x + object->Width();
				m_pos.x = newPosX;
				m_vel.x = 0.0f;
				m_grounded = true;
				m_bonus_jump = true;
				// on the right 
			}
			else
			{
				float newPosY = object->GetPos().y - m_size.y;
				m_pos.y = newPosY;
				m_vel.y = 0.0f;
				m_grounded = true;
				m_bonus_jump = true;
				// at the top 
			}
		}
	}
	else
	{
		m_grounded = false;
	}

}
