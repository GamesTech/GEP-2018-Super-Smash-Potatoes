#pragma once
#include "ImageGO2D.h"

enum Direction
{
	LEFT,
	RIGHT
};

enum ActionJump
{
	GROUND,
	JUMP,
	UPWARDPUNCH,
	PUNCH,
	FALL,
	HIT
};

enum ActionMovement
{
	STILL,
	WALK,
	RUN,
	GRAB
};

//GEP:: Animation class
class Animation2D :
	public ImageGO2D
{
public:
	Animation2D(RenderData* _RD, string _filename);
	~Animation2D();

	void LoadSprites(string _filename);
	void AnimationTick(GameStateData * _GSD);
	void AnimationOn();
	void Punch();

protected:
	Direction direction;
	ActionJump action_jump;
	ActionMovement action_movement;
	bool m_animation_on = false;

private:
	void SetDefault(int animation);
	void SetAnimationSprite(int action);
	void SetJump();
	void SetJumpPunch();
	void SetWalk(int animation);
	void SetRun();
	void SetFall();
	void SetPunch(int animation);
	void SetGrab();
	void SetHit();

	enum SpriteStateData
	{
		LDefault,
		RDefault,
		LDefault2,
		RDefault2,
		LJump,
		RJump,
		LWalk1,
		RWalk1,
		LWalk2,
		RWalk2,
		LWalk3,
		RWalk3,
		LFall,
		RFall,
		RGrab,
		LGrab,
		UpwardJump,
		LKick1,
		RKick1,
		LKick2,
		RKick2,
		LKick3,
		RKick3,
		LHit,
		RHit
	};


	float timer = 0;
	float timer2 = 0;
	float timer_punch = 0.1;
	int m_change_punch_animation = 1;
	int m_change_animation = 1;
	int m_change_animation2 = 1;
	int m_pixel_gap_colour = 0;
	int m_player_number = 0;
	float sprite_batch[26][4];
};