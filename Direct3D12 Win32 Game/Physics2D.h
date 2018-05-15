#pragma once
#include "Animation2D.h"

//GEP:: Euler Integration Solve for VERY Basic 2D physics
class Physics2D :
	public Animation2D
{
public:
	enum Collision
	{
		COLNONE,
		COLTOP,
		COLBOTTOM,
		COLLEFT,
		COLRIGHT,
	};
	
	Physics2D(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer);
	~Physics2D();

	void			AddForce	(Vector2 _push) { m_acc += (_push / m_mass); }
	void			AddGravity	(bool _grounded);
	virtual void	Tick		(GameStateData* _GSD);
	void			ProcessCollision();

	Vector2		GetVel()	{ return m_vel; };
	float		GetMass()	{ return m_mass; };
	float		GetDrag()	{ return m_drag; };
	
	bool		IgnoreCollision()	{ return m_ignore_collision; };
	bool		GetLedgeJump()		{ return m_ledge_jump; };

	void		SetNewPos	(float _new_pos)		{ m_new_pos = _new_pos; };
	void		SetMass		(float _mass)			{ m_mass = _mass; };
	void		SetDrag		(float _drag)			{ m_drag = _drag; };
	void		SetSpeedLimit(float _speed_limit)	{ m_speed_limit = _speed_limit; };
	void		SetCollState(Collision _col)		{ m_coll_state = _col; };


protected:
	Vector2		m_vel;
	Vector2		m_acc;
	Vector2		m_gravity = Vector2 (0.f, 2000.f);
	Collision	m_coll_state = COLNONE;

	float m_mass = 1.0f;
	float m_drag = 0.0f;
	float m_speed_limit = 1;
	float m_new_pos = 1.f;
	
	int	m_jump_count = 0;

	// collison bools
	bool m_bonus_jump = false;
	bool m_grabing_side = false;
	bool m_ledge_jump = false;
	
	bool m_grounded = false;
	bool m_ignore_collision = false;
	
	bool m_y_coll = false;
	bool m_x_coll = false;
};

