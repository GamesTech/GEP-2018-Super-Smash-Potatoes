#pragma once
#include "Physics2D.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include "PlayerFile.h"
#include "Input.h"
#include "Item.h"

//GEP:: Based on the ImageGO2D a basic keyboard controlled sprite
enum Attack
{
	NONE,
	FIRST,
	SECOND,
	THIRD,
};

class Player2D :
	public Physics2D
{
public:

	Player2D(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer, string _file);
	virtual ~Player2D();
	void	Tick(GameStateData* _GSD, int _test, Input* input_manager);

	// setters
	void	setPlayerNo		(int player_number) { player_no = player_number; };
	void	SetSpawnPosition(Vector2 spawn)		{ m_respawn_pos = spawn; };
	void	SetDamage		(float damage)		{ m_damage = damage; };
	// ?? My bad, I was probably drunk

	void	SetImmune		(bool _immune)			{ m_immune = _immune; };
	void	SetAnimGrounded	(bool _anim_grounded)	{ m_anim_grounded = _anim_grounded; };
	
	void	SetParticleSystem	(std::shared_ptr<ParticleSystem> ps) { particle_system = ps; };
	void	SetCollState		(Collision _col)					 { m_coll_state = _col; };
	void	setItem				(Item* item)                         { this->item = item; };
	void	ResetAttacks()											 { m_execute_attack = Attack::NONE; };

	// getters
	float	GetDamage()			{ return m_damage; };
	
	int		GetDirection()		{ return m_direction; };
	int		GetLivesRemaining() { return lives_remaining; };
	int		getPlayerNo()		{ return player_no; };

	bool	GetImmune()			{ return m_immune; };
	bool	GetInvincibility()	{ return m_invincibility; };
	bool	getDead()			{ return m_dead; };

	Attack	GetAttackType() { return m_execute_attack; };
	Item*	getItem()const	{ return item; };

	// attack functions
	bool	CheckBlocking	(GameStateData * _GSD, Player2D* other_player);
	bool	ExectuePunch	(GameStateData * _GSD, Player2D* other_player);
	bool	ExectueUpPunch	(GameStateData * _GSD, Player2D* other_player);
	bool	ExectueDownPunch(GameStateData * _GSD, Player2D* other_player);	
	
	void	GotHit			(GameStateData * _GSD, float _dir, float y_force);
	void	Block			(GameStateData * _GSD);

protected:
	
	void	SetStatsFromFile();
	void	Grabbing();
	void	updateOrientation();
	
	void	AnimationChecks(GameStateData * _GSD);
	void	deathZone      (GameStateData * _GSD);
	void	respawn        (GameStateData * _GSD);
	void    controller     (Input * input_manager, GameStateData * _GSD);

	// timers
	void	HitTimer	(GameStateData * _GSD);
	void	CollTimer	(GameStateData * _GSD);
	void    PunchTimer	(GameStateData * _GSD);
	void	UpPunchTimer(GameStateData * _GSD);
	void	RespawnTimer(GameStateData * _GSD);

	// animation bools
	bool	m_jumping_anim = false;
	bool	m_up_punch_anim	= false;
	bool	m_down_punching_anim = false;
	bool	m_punch_anim = false;
	bool	m_anim_grounded = false;
	
	// player state bools
	bool	m_remove_controll = false;
	bool	m_dead = false;
	bool	m_immune = false;
	bool	m_invincibility = false;

	bool	punch_particle = true;

	// timer floats
	float	m_timer_punch = 4;
	float	m_up_timer_punch = 4;
	float	m_timer_hit	= 4;
	float	m_respawn_timer = 3;
	float	m_ignore_coll_timer	= 3;

	// player stats
	float	m_jump_force = 60000.f;
	float	m_jump_upwards_force = 100000.f;
	float   m_slam_force = 60000.f;
	float   m_hit_force = 60000.f;
	float	m_drive = 200.0f;
	float	m_damage = 1;
	float	m_direction = 1;
	
	int		player_no = 0;
	int		m_max_jumps = 3;
	int		lives_remaining = 3;

	Vector2	m_limit = Vector2(1780, 1220);
	Vector2 m_respawn_pos = Vector2(600, 250);
	Vector2	m_max_speed	= Vector2(400, 400);
	
	Attack  m_execute_attack = NONE;
	
	Item*	item = nullptr;

	std::shared_ptr<ParticleSystem> particle_system = nullptr;
	std::unique_ptr<PlayerFile>		player_file	= nullptr;
};
