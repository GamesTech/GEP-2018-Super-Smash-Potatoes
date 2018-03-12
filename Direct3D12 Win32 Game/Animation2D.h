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
	FALL
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
	void SetDefault();
	void SetJump();
	void SetWalk(int animation);
	void SetWalk1();
	void SetWalk2();
	void SetRun();
	void SetFall();
	void SetPunch();
	void SetGrab();

protected:
	Direction direction;
	ActionJump action_jump;
	ActionMovement action_movement;
	bool m_animation_on = false;

private:
	enum SpriteStateData
	{
		LDefault,
		RDefault,
		LJump,
		RJump,
		LWalk1,
		RWalk1,
		LWalk2,
		RWalk2,
		LWalk3,
		RWalk3,
		Fall,
		LGrab,
		RGrab
	};


	float timer = 0;
	int m_change_animation = 1;
	int m_pixel_gap_colour = 0;
	int m_player_number = 0;
	float sprite_batch[13][4];
};