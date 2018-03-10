#pragma once
#include "Physics2D.h"

//GEP:: Based on the ImageGO2D a basic keyboard controlled sprite
class Player2D :
	public Physics2D
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

	//TODO: add a 3d player and modes to switch between different views and basic physics
	Player2D(RenderData* _RD, string _filename);
	virtual ~Player2D();

	void Tick(GameStateData* _GSD/*, GameObject2D* _obj*/);

	void setPlayerNo(int player_number);

	void respawn();

	void controller(GameStateData * _GSD);

	void SetDrive(float _drive) { m_drive = _drive; }
	float GetDrive() { return m_drive; }
	void SetVelY(float _vel_y) { m_vel.y = _vel_y; }
	void SetVelX(float _vel_x) { m_vel.x = _vel_x; }
	void SetAnimGrounded(bool _anim_grounded) { m_anim_grounded = _anim_grounded; };
	bool GetGrounded() { return m_grounded; };
	void SetNewPos(float _new_pos) { m_new_pos = _new_pos; };

	void TestCollision() { SetBoundingBoxes(); }
	void CheckCollision(GameObject2D* _obj);
	void SetLimit(Vector2 _lim) { m_limit = _lim; }
	void SetSpeedLimit(float _speed_limit) { m_speed_limit = _speed_limit; };
	void SetLayer(float _new_pos) { m_new_pos = _new_pos; }
	Vector2 GetLimit() { return m_limit; };
	Vector2 GetCurrVel() { return m_vel; };
	void SetCollState(Collision _col) { m_coll_state = _col; };
	Collision GetCollState() { return m_coll_state; };

protected:
	void ProcessCollision();

	float m_jumpForce = 120000;
	float m_drive = 200.0f;
	//float m_gravity = 980;
	int player_no = 0;
	Vector2 m_max_speed = Vector2(400, 400);

	bool m_grounded = false;
	bool m_bonus_jump = false;
	bool m_anim_grounded = false;
	bool m_y_coll = false;
	bool m_x_coll = false;
	//bool m_jumping = false;
 	//float m_speed_limit;
	float m_new_pos = 0;
	Collision m_coll_state = COLNONE;

	Vector2 m_limit = Vector2(1280, 720);
};

