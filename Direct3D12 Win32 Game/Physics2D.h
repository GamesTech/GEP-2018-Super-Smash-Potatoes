#pragma once
#include "ImageGO2D.h"

//GEP:: Euler Integration Solve for VERY Basic 2D physics
class Physics2D :
	public ImageGO2D
{
public:
	Physics2D(RenderData* _RD, string _filename);
	~Physics2D();

	void AddForce(Vector2 _push) { m_acc += _push / m_mass; }

	Vector2 GetVel() { return m_vel; }

	void SetMass(float _mass) { m_mass = _mass; }
	float GetMass() { return m_mass; }
	void SetDrag(float _drag) { m_drag = _drag; }
	float GetDrag() { return m_drag; }

	virtual void Tick(GameStateData* _GSD);

protected:
	Vector2 m_vel;
	Vector2 m_acc;

	float m_mass = 1.0f;
	float m_drag = 0.0f;
};

