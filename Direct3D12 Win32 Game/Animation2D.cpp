#include "pch.h"
#include "GameStateData.h"
#include "Animation2D.h"

#include<iostream>
#include<fstream>

Animation2D::Animation2D(RenderData* _RD, string _filename) :ImageGO2D(_RD, _filename)
{
	action_movement = STILL;
	direction = LEFT;
}

Animation2D::~Animation2D()
{

}

void Animation2D::AnimationTick(GameStateData * _GSD)
{

	if (action_jump == GROUND)
	{
		if (action_movement == WALK)
		{
			SetWalk(m_change_animation);
		}
		else if (action_movement == GRAB)
		{
			SetGrab();
		}
		else
		{
			SetDefault(m_change_animation2);
		}
	}
	else if (action_jump == PUNCH)
	{
		SetPunch(m_change_punch_animation);
	}
	else if (action_jump == JUMP)
	{
		SetJump();
	}
	else if (action_jump == UPWARDPUNCH)
	{
		SetJumpPunch();
	}
	else if (action_jump == DOWNWARDPUNCH)
	{
		SetDownPunch();
	}
	else if (action_jump == HIT)
	{
		SetHit();
	}
	else if (action_jump == FALL)
	{
		SetFall();
	}

	AnimationTimers(_GSD);
}

void Animation2D::AnimationTimers(GameStateData * _GSD)
{
	//Timer for the walk animation
	if (timer >= 0.2)
	{
		if (m_change_animation < 3)
		{
			m_change_animation++;
		}
		else
		{
			m_change_animation = 1;
		}
		timer -= 0.2;
	}
	else
	{
		timer += _GSD->m_dt;
	}
	//Timer for the character to blink
	if (timer2 >= 10)
	{
		if (m_change_animation2 < 2)
		{
			m_change_animation2++;
			timer2 -= 0.3;
		}
		else
		{
			m_change_animation2 = 1;
			timer2 -= rand() % 10;
		}

	}
	else
	{
		timer2 += _GSD->m_dt;
	}
	//Timer for the punch animation
	if (timer_punch >= 0.1)
	{
		if (m_change_punch_animation < 4)
		{
			m_change_punch_animation++;
			timer2 -= 0.1;
		}
	}
	else
	{
		timer_punch += _GSD->m_dt;
	}
}

void Animation2D::AnimationOn()
{
	m_animation_on = true;
}

void Animation2D::Punch()
{
	timer_punch = 0;
	m_change_punch_animation = 1;
}

void Animation2D::SetDefault(int animation)
{
	if (animation == 1)
	{
		if (action_movement == STILL)
		{
			if (direction == LEFT)
			{
				SetAnimationSprite(LDefault);
			}
			if (direction == RIGHT)
			{
				SetAnimationSprite(RDefault);
			}
		}
	}
	else
	{
		if (action_movement == STILL)
		{
			if (direction == LEFT)
			{
				SetAnimationSprite(LDefault2);
			}
			if (direction == RIGHT)
			{
				SetAnimationSprite(RDefault2);
			}
		}
	}
}

void Animation2D::SetJump()
{
	if (direction == LEFT)
	{
		SetAnimationSprite(LJump);
	}
	if (direction == RIGHT)
	{
		SetAnimationSprite(RJump);
	}
}

void Animation2D::SetJumpPunch()
{
	SetAnimationSprite(UpwardJump);
}

void Animation2D::SetDownPunch()
{
	if (direction == LEFT)
	{
		SetAnimationSprite(LDownwardPunch);
	}
	if (direction == RIGHT)
	{
		SetAnimationSprite(RDownwardPunch);
	}
}

void Animation2D::SetWalk(int animation)
{
	if (animation == 1)
	{
		if (direction == LEFT)
		{
			SetAnimationSprite(LWalk1);
		}
		if (direction == RIGHT)
		{
			SetAnimationSprite(RWalk1);
		}
	}
	else if (animation == 2)
	{
		if (direction == LEFT)
		{
			SetAnimationSprite(LWalk2);
		}
		if (direction == RIGHT)
		{
			SetAnimationSprite(RWalk2);
		}
	}
	else
	{
		if (direction == LEFT)
		{
			SetAnimationSprite(LWalk3);
		}
		if (direction == RIGHT)
		{
			SetAnimationSprite(RWalk3);
		}
	}
}


void Animation2D::SetRun()
{
}

void Animation2D::SetFall()
{
	if (direction == LEFT)
	{
		SetAnimationSprite(LFall);
	}
	if (direction == RIGHT)
	{
		SetAnimationSprite(RFall);
	}
}

void Animation2D::SetPunch(int animation)
{
	if (animation == 1)
	{
		if (direction == LEFT)
		{
			SetAnimationSprite(LKick1);
		}
		if (direction == RIGHT)
		{
			SetAnimationSprite(RKick1);
		}
	}
	else if (animation == 2)
	{
		if (direction == LEFT)
		{
			SetAnimationSprite(LKick3);
		}
		if (direction == RIGHT)
		{
			SetAnimationSprite(RKick2);
		}
	}
	else
	{
		if (direction == LEFT)
		{
			SetAnimationSprite(LKick3);
		}
		if (direction == RIGHT)
		{
			SetAnimationSprite(RKick3);
		}
	}
}

void Animation2D::SetGrab()
{
	if (direction == LEFT)
	{
		SetAnimationSprite(LGrab);
	}
	if (direction == RIGHT)
	{
		SetAnimationSprite(RGrab);
	}
}

void Animation2D::SetHit()
{
	if (direction == LEFT)
	{
		SetAnimationSprite(LHit);
	}
	if (direction == RIGHT)
	{
		SetAnimationSprite(RHit);
	}
}

void Animation2D::SetAnimationSprite(int action)
{
	SetRect(sprite_batch[action][0], sprite_batch[action][1], sprite_batch[action][2], sprite_batch[action][3]);
}

void Animation2D::LoadSprites(string _filename)
{
	std::ifstream sprite_position_batching;
	sprite_position_batching.open(_filename);
	if (sprite_position_batching.is_open())
	{
		while (!sprite_position_batching.eof())
		{
			for (int j = 0; j < 27; ++j)
			{
				for (int i = 0; i < 4; ++i) //prints into array Default Left
				{
					sprite_position_batching >> sprite_batch[j][i];
				}
			}			
		}
	}
	sprite_position_batching.close();
	SetAnimationSprite(LDefault);
}