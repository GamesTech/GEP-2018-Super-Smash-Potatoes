#pragma once
#include "ImageGO2D.h"

enum ActionJump
{
	GROUND,
	JUMP,
	UPWARDPUNCH,
	DOWNWARDPUNCH,
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
	Animation2D(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer);
	~Animation2D();

	void LoadSprites(string _filename);
	void AnimationTick(GameStateData * _GSD);
	void AnimationTimers(GameStateData * _GSD);
	void AnimationOn();
	void Punch();

protected:
	ActionJump action_jump;
	ActionMovement action_movement;
	bool m_animation_on = false;

private:
	void SetDefault(int animation);
	void SetAnimationSprite(int action);
	void SetJump();
	void SetJumpPunch();
	void SetDownPunch();
	void SetWalk(int animation);
	void SetRun();
	void SetFall();
	void SetPunch(int animation);
	void SetGrab();
	void SetHit();

	enum SpriteStateData
	{
		CharacterWidth,
		Default,
		Default2,
		Jump,
		Walk1,
		Walk2,
		Walk3,
		Fall,
		Grab,
		UpwardJump,
		Kick1,
		Kick2,
		Kick3,
		Hit,
		DownwardPunch
	};


	float timer = 0;
	float timer2 = 0;
	float timer_punch = 0.1;
	int m_change_punch_animation = 1;
	int m_change_animation = 1;
	int m_change_animation2 = 1;
	int m_pixel_gap_colour = 0;
	int m_player_number = 0;
	float sprite_batch[15][4];
};