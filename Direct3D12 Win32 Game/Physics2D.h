#pragma once
#include "Animation2D.h"

//GEP:: Euler Integration Solve for VERY Basic 2D physics
class Physics2D :
	public Animation2D
{
public:
	Physics2D(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer);
	~Physics2D();

	void AddForce(Vector2 _push) { m_acc += (_push / m_mass); }
	void AddGravity(bool _grounded);

	Vector2 GetVel() { return m_vel; }

	void SetMass(float _mass) { m_mass = _mass; }
	float GetMass() { return m_mass; }
	void SetDrag(float _drag) { m_drag = _drag; }
	float GetDrag() { return m_drag; }
	void SetSpeedLimit(float _speed_limit) { m_speed_limit = _speed_limit; };

	virtual void Tick(GameStateData* _GSD, bool _coll_y, bool _coll_x, float _new_pos, bool _grabbing);

protected:
	Vector2 m_vel;
	Vector2 m_acc;
	Vector2 m_gravity = Vector2 (0.f, 2000.f);

	float m_mass = 1.0f;
	float m_drag = 0.0f;
	float m_speed_limit = 1;
	bool m_grounded = false;
};

