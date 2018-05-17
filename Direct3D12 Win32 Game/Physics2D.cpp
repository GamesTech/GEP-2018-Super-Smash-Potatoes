#include "pch.h"
#include "Physics2D.h"
#include "GameStateData.h"

Physics2D::Physics2D(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer):Animation2D(_RD,_filename, image_buffer)
{
}


Physics2D::~Physics2D()
{
}

// basic gravity 
void Physics2D::AddGravity(bool _grounded)
{
	m_grounded = _grounded;
	if (!m_grounded)
	{
		m_acc += (m_gravity * m_mass);
	}

}

//GEP:: Basic Euler Solver for point mass 
void Physics2D::Tick(GameStateData * _GSD)
{
	ProcessCollision();

	if (m_grounded)
	{
		m_acc -= (m_drag * m_vel);
	}
	else
	{
		m_acc -= (1.f * m_vel);
	}

	Vector2 newPos = m_pos + _GSD->m_dt * m_vel;
	Vector2 newVel = m_vel + _GSD->m_dt * m_acc;

	if (m_y_coll)
	{
		newVel.y = -newVel.y * m_bounciness;
		newPos.y = m_new_pos;
	}
	else if(m_x_coll)
	{
		newVel.x = -newVel.x * m_bounciness;
		newPos.x = m_new_pos;
	}
	if (m_grabing_side)
	{
		if (newVel.y > 200)
		{
			newVel.y = 200;
		}
	}


	m_pos = newPos;
	m_vel = newVel;
	m_acc = Vector2::Zero;
}

void Physics2D::ProcessCollision()
{
	switch (m_coll_state)
	{
	case COLTOP:
		m_grounded = true;
		m_jump_count = 0;
		m_bonus_jump = true;
		m_y_coll = true;
		m_x_coll = false;
		break;
	case COLBOTTOM:
		m_y_coll = true;
		m_x_coll = false;
		break;
	case COLRIGHT:
		m_grounded = true;
		m_x_coll = true;
		m_y_coll = false;
		break;
	case COLLEFT:
		m_grounded = true;
		m_x_coll = true;
		m_y_coll = false;
		break;
	case COLNONE:
		m_x_coll = false;
		m_y_coll = false;
		m_grounded = false;
		break;
	}
}
