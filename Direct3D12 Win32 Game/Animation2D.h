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

	void loadSprites(string _filename);
	void AnimationTick();
	void AnimationOn();
	void SetDefault();
	void SetJump();
	void SetWalk();
	void SetWalk1();
	void SetWalk2();
	void SetRun();
	void SetFall();
	void SetPunch();

protected:
	Direction direction;
	ActionJump action_jump;
	ActionMovement action_movement;
	bool m_animation_on = false;

private:
	float left_default_positions[4] = { 0, 0, 0, 0 };
	float right_default_positions[4] = { 0, 0, 0, 0 };
	float left_jump_positions[4] = { 0, 0, 0, 0 };
	float right_jump_positions[4] = { 0, 0, 0, 0 };
	float left_walk_positions[4] = { 0, 0, 0, 0 };
	float right_walk_positions[4] = { 0, 0, 0, 0 };
	float left_walk_1_positions[4] = { 0, 0, 0, 0 };
	float right_walk_1_positions[4] = { 0, 0, 0, 0 };
	float left_walk_2_positions[4] = { 0, 0, 0, 0 };
	float right_walk_2_positions[4] = { 0, 0, 0, 0 };
	float fall_positions[4] = { 0, 0, 0, 0 };

};