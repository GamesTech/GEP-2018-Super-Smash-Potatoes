#include "pch.h"
#include "Player2D.h"
#include "GameStateData.h"

Player2D::Player2D(RenderData* _RD, string _filename) :Physics2D(_RD, _filename)
{
	SetBoundingBoxes();
	BottomOrigin();
	//SetMass(100);
}


Player2D::~Player2D()
{
}

void Player2D::Tick(GameStateData * _GSD, GameObject2D* _obj)
{

//Push the guy around in the directions for the key presses
	SetBoundingBoxes();

	if (m_pos.y < m_limit.y)
	{
		m_grounded = false;
	}
	else
	{
		m_grounded = true;
	}

	CheckCollision(_obj);
	
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
		if (m_grounded)
		{
			AddForce(-m_jumpForce * Vector2::UnitY);
		}
	}

	AddGravity(m_grounded);

	Vector2 mousePush = Vector2(_GSD->m_mouseState.x, _GSD->m_mouseState.y);
	
	//AddForce(m_drive*mousePush);

	//GEP:: Lets go up the inheritence and share our functionality

	Physics2D::Tick(_GSD);

	//after that as updated my position let's lock it inside my limits
	if (m_pos.x < 0.0f)
	{
		m_pos.x = 0.0f;
		m_vel.x = 0.0f; // yea, not a nice bounce for works okay for a first pass
	}
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;
		m_vel.y = 0.0f;
	}

	if (m_pos.x > m_limit.x)
	{
		m_pos.x = m_limit.x;
		m_vel.x = 0.0f;
	}
	if (m_pos.y > m_limit.y)
	{
		m_pos.y = m_limit.y;
		m_vel.y = 0.0f;
	}
}

void Player2D::CheckCollision(GameObject2D *_obj)
{
	GameObject2D* object = _obj;
	if (object->CollMin().x >= m_min.x
		&& object->CollMax().x <= m_max.x
		&& object->CollMin().y >= m_min.y
		&& object->CollMax().y <= m_max.y)
	{
		m_grounded = m_pos.x = object->CollMax().x;
	}
	if (m_max.x <= object->CollMax().x && m_max.x >= object->CollMin().x &&  m_pos.y <= object->CollMax().y)
	{
		m_pos.x = object->CollMin().x - m_size;
		m_vel.x = 0.0f; // yea, not a nice bounce for works okay for a first pass
	}
	if (m_pos.x <= object->CollMax().x && m_pos.x >= object->CollMin().x && m_pos.y <= object->CollMax().y)
	{
		m_pos.x = object->CollMax().x;
		m_vel.x = 0.0f; // yea, not a nice bounce for works okay for a first pass
	}

	//if (m_pos.x <= object->CollMax().y && m_pos.x >= object->CollMin().y)
	//{
	//	m_pos.x = object->CollMax().x;
	//	m_vel.x = 0.0f; // yea, not a nice bounce for works okay for a first pass
	//}

	//if (m_pos.y < 0.0f)
	//{
	//	m_pos.y = 0.0f;
	//	m_vel.y = 0.0f;
	//}

	//if (m_pos.x > m_limit.x)
	//{
	//	m_pos.x = m_limit.x;
	//	m_vel.x = 0.0f;
	//}
	//if (m_pos.y > m_limit.y)
	//{
	//	m_pos.y = m_limit.y;
	//	m_vel.y = 0.0f;
	//}

}
