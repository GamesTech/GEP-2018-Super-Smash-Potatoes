#include "pch.h"
#include "Physics2D.h"
#include "GameStateData.h"


Physics2D::Physics2D(RenderData* _RD, string _filename):Animation2D(_RD,_filename)
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
void Physics2D::Tick(GameStateData * _GSD, bool _coll_y, bool _coll_x, float _new_pos, bool _grabbing)
{
	//VERY Basic idea of drag i.e. the faster I go the more I get pulled back
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

	if (_coll_y)
	{
		//newPos.y += (m_pos.y + _GSD->m_dt * m_vel.y)*-1;
		newVel.y += (m_vel.y + _GSD->m_dt * m_acc.y)*-1;
		newPos.y = _new_pos;
	}
	else if(_coll_x)
	{
		//newPos.y += (m_pos.y + _GSD->m_dt * m_vel.y)*-1;
		newVel.x += (m_vel.x + _GSD->m_dt * m_acc.x)*-1;
		newPos.x = _new_pos;
	}
	if (_grabbing)
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
