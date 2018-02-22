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
	RUN
};

//GEP:: Animation class
class Animation2D :
	public ImageGO2D
{
public:
	Animation2D(RenderData* _RD, string _filename);
	~Animation2D();

	void AnimationTick();
	void AnimationOn();
	void SetDefault();
	void SetJump();
	void SetWalk();
	void SetRun();
	void SetFall();
	void SetPunch();

protected:
	Direction direction;
	ActionJump action_jump;
	ActionMovement action_movement;
	bool m_animation_on = false;

};