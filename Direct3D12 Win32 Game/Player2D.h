#pragma once
#include "Physics2D.h"

//GEP:: Based on the ImageGO2D a basic keyboard controlled sprite
class Player2D :
	public Physics2D
{
public:

	//TODO: add a 3d player and modes to switch between different views and basic physics
	Player2D(RenderData* _RD, string _filename);
	virtual ~Player2D();

	void Tick(GameStateData* _GSD, GameObject2D* _obj);

	void SetDrive(float _drive) { m_drive = _drive; }
	float GetDrive() { return m_drive; }

	void TestCollision() { SetBoundingBoxes(); }
	void CheckCollision(GameObject2D* _obj);
	void SetLimit(Vector2 _lim) { m_limit = _lim; }
	Vector2 GetLimit() { return m_limit; }

protected:
	//float m_gravity = 980;
	float m_jumpForce = 45000;
	float m_drive = 100.0f;
	Vector2 m_max_speed = Vector2(400, 400);

	bool m_grounded = false;

	Vector2 m_limit = Vector2(1100, 600);
};

