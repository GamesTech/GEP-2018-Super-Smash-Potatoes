#pragma once
#include "ImageGO2D.h"

enum Facing
{
	LEFT,
	RIGHT
};

enum Action
{
	NONE,
	WALK,
	RUN,
	JUMP,
	FALL,
	PUNCH
};

//GEP:: Animation class
class Animation2D :
	public ImageGO2D
{
public:
	Animation2D(RenderData* _RD, string _filename);
	~Animation2D();

	void AnimationOn();
	void SetJump();
	void SetWalk();
	void SetRun();
	void SetFall();
	void SetPunch();

protected:
	bool m_animation_on = false;

};