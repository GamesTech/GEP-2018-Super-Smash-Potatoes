#pragma once
#include "Physics2D.h"
#include "ParticleSystem.h"
#include "Particle.h"

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

	//void init(AudioManager* am);

	void Tick(GameStateData* _GSD, int _test/*, GameObject2D* _obj*/);

	void setPlayerNo(int player_number);

	void SetDrive(float _drive) { m_drive = _drive; }
	//float GetDrive() { return m_drive; }
	//bool GetGrounded() { return m_grounded; };
	
	//void SetVelY(float _vel_y) { m_vel.y = _vel_y; }
	//void SetVelX(float _vel_x) { m_vel.x = _vel_x; }
	
	void SetAnimGrounded(bool _anim_grounded) { m_anim_grounded = _anim_grounded; };
	void SetNewPos(float _new_pos) { m_new_pos = _new_pos; };

	//void TestCollision() { SetBoundingBoxes(); }
	//void SetLimit(Vector2 _lim) { m_limit = _lim; }
	//void SetSpeedLimit(float _speed_limit) { m_speed_limit = _speed_limit; };
	void SetLayer(float _new_pos) { m_new_pos = _new_pos; }

	//Vector2 GetLimit() { return m_limit; };
	Vector2 GetCurrVel() { return m_vel; };

	void SetCollState(Collision _col) { m_coll_state = _col; };
	//Collision GetCollState() { return m_coll_state; };

	void SetParticleSystem(std::shared_ptr<ParticleSystem> ps) { particle_system = ps; };

	bool GetLedgeJump() { return m_ledge_jump; };
	bool IsPunching() { return m_execute_punch; };
	bool IsUpPuching() { return m_execute_up_punch; };
	void ResetAttacks(bool _attack) { m_execute_punch = _attack; m_execute_up_punch = _attack;};
	bool IgnoreCollision() { return m_ignore_collision; };

	float GetDamage() { return m_damage; };

	bool CheckBlocking(GameStateData * _GSD, Player2D* other_player);
	bool ExectuePunch(GameStateData * _GSD, Player2D* other_player);
	bool ExectueUpPunch(GameStateData * _GSD, Player2D* other_player);
	void GotHit(GameStateData * _GSD, int _dir, int y_force);
	void SetImmune(bool _immune) { m_immune = _immune; };
	void Block(GameStateData * _GSD);
	bool GetOrientation();
	bool GetImmune() { return m_immune; }
	bool GetInvincibility() { return m_invincibility; };

	//void SetLivesRemaining(int lives) { lives_remaining = lives; };
	int GetLivesRemaining() { return lives_remaining; };

	bool getDead() { return m_dead; };

protected:
	void ProcessCollision();
	void updateOrientation();
	void AnimationChecks(GameStateData * _GSD);
	void HitTimer(GameStateData * _GSD);
	void Grabbing();
	void PunchTimer(GameStateData * _GSD);
	void UpPunchTimer(GameStateData * _GSD);
	void deathZone();
	void respawn();
	void RespawnTimer(GameStateData * _GSD);
	void controller(GameStateData * _GSD);

	float m_jumpForce = 60000;
	float m_drive = 200.0f;
	float m_damage = 1;
	//float m_gravity = 980;
	int player_no = 0;
	int lives_remaining = 3;
	Vector2 m_max_speed = Vector2(400, 400);

	bool m_grounded = false;
	bool m_execute_up_punch = false;
	bool m_jumping = false;
	bool m_up_punching = false;
	bool m_down_punching = false;
	bool m_punching = false;
	bool m_bonus_jump = false;
	bool m_anim_grounded = false;
	bool m_grabing_side = false;
	bool m_ledge_jump = false;
	bool m_y_coll = false;
	bool m_x_coll = false;
	bool m_execute_punch = false;
	bool m_remove_controll = false;
	bool m_dead = false;
	bool m_ignore_collision = false;
	bool m_immune = false;
	bool m_invincibility = false;

	float m_timer_punch = 4;
	float m_up_timer_punch = 4;
	float m_timer_hit = 4;
	float m_respawn_timer = 3;

	int m_direction = 1;

	//bool m_jumping = false;
 	//float m_speed_limit;
	float m_new_pos = 0;
	Collision m_coll_state = COLNONE;

	Vector2 m_limit = Vector2(1280, 720);
	std::shared_ptr<ParticleSystem> particle_system = nullptr;
};

