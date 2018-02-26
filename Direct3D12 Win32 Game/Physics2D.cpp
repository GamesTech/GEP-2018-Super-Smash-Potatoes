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
	if (true)
	{
		m_acc += (m_gravity * m_mass) / m_speed_limit;
	}

}

//GEP:: Basic Euler Solver for point mass 
void Physics2D::Tick(GameStateData * _GSD)
{
	//VERY Basic idea of drag i.e. the faster I go the more I get pulled back
	if (m_grounded)
	{
		m_acc -= (m_drag * m_vel) / m_speed_limit;
	}
	else
	{
		m_acc -= (1.f * m_vel) / m_speed_limit;
	}

	Vector2 newPos = m_pos + _GSD->m_dt * m_vel;
	Vector2 newVel = m_vel + _GSD->m_dt * m_acc;

	m_pos = newPos;
	m_vel = newVel;
	m_acc = Vector2::Zero;
}
