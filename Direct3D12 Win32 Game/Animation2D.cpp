#include "pch.h"
#include "Animation2D.h"

Animation2D::Animation2D(RenderData* _RD, string _filename) :ImageGO2D(_RD, _filename)
{
	action_movement = STILL;
}

Animation2D::~Animation2D()
{

}

void Animation2D::AnimationTick()
{
	
	if (action_jump == GROUND)
	{
		if (action_movement == WALK)
		{
			SetWalk();
		}
		else
		{
			SetDefault();
		}
	}
	else if (action_jump == JUMP)
	{
		SetJump();
	}
	
}

void Animation2D::AnimationOn()
{
	m_animation_on = true;
}

void Animation2D::SetDefault()
{
	if (action_movement == STILL)
	{
		if (direction == LEFT)
		{
			SetRect(724, 0, 775, 63);
		}
		if (direction == RIGHT)
		{
			SetRect(844, 0, 894, 63);
		}
	}
}

void Animation2D::SetJump()
{
	if (direction == LEFT)
	{
		SetRect(116, 0, 182, 63);
	}
	if (direction == RIGHT)
	{
		SetRect(1435, 0, 1502, 63);
	}
}

void Animation2D::SetWalk()
{
	if (direction == LEFT)
	{
		SetRect(600, 0, 655, 63);
	}
	if (direction == RIGHT)
	{
		SetRect(964, 0, 1019, 63);
	}
}

void Animation2D::SetRun()
{
}

void Animation2D::SetFall()
{
}

void Animation2D::SetPunch()
{
}
