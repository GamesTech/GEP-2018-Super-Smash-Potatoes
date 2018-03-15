#include "pch.h"
#include "GameStateData.h"
#include "Animation2D.h"

#include<iostream>
#include<fstream>

Animation2D::Animation2D(RenderData* _RD, string _filename) :ImageGO2D(_RD, _filename)
{
	action_movement = STILL;
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
	else if (action_jump == FALL)
	{
		SetFall();
	}


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

	if (timer_punch >= 0.4)
	{
		if (m_change_punch_animation < 4)
		{
			m_change_punch_animation++;
			timer2 -= 0.4;
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
				SetRect(sprite_batch[LDefault][0], sprite_batch[LDefault][1], sprite_batch[LDefault][2], sprite_batch[LDefault][3]);
			}
			if (direction == RIGHT)
			{
				SetRect(sprite_batch[RDefault][0], sprite_batch[RDefault][1], sprite_batch[RDefault][2], sprite_batch[RDefault][3]);
			}
		}
	}
	else
	{
		if (action_movement == STILL)
		{
			if (direction == LEFT)
			{
				SetRect(sprite_batch[LDefault2][0], sprite_batch[LDefault2][1], sprite_batch[LDefault2][2], sprite_batch[LDefault2][3]);
			}
			if (direction == RIGHT)
			{
				SetRect(sprite_batch[RDefault2][0], sprite_batch[RDefault2][1], sprite_batch[RDefault2][2], sprite_batch[RDefault2][3]);
			}
		}
	}
}

void Animation2D::SetJump()
{
	if (direction == LEFT)
	{
		SetRect(sprite_batch[LJump][0], sprite_batch[LJump][1], sprite_batch[LJump][2], sprite_batch[LJump][3]);
	}
	if (direction == RIGHT)
	{
		SetRect(sprite_batch[RJump][0], sprite_batch[RJump][1], sprite_batch[RJump][2], sprite_batch[RJump][3]);
	}
}

void Animation2D::SetJumpPunch()
{
	SetRect(sprite_batch[UpwardJump][0], sprite_batch[UpwardJump][1], sprite_batch[UpwardJump][2], sprite_batch[UpwardJump][3]);
}

void Animation2D::SetWalk(int animation)
{
	if (animation == 1)
	{
		if (direction == LEFT)
		{
			SetRect(sprite_batch[LWalk1][0], sprite_batch[LWalk1][1], sprite_batch[LWalk1][2], sprite_batch[LWalk1][3]);
		}
		if (direction == RIGHT)
		{
			SetRect(sprite_batch[RWalk1][0], sprite_batch[RWalk1][1], sprite_batch[RWalk1][2], sprite_batch[RWalk1][3]);
		}
	}
	else if (animation == 2)
	{
		if (direction == LEFT)
		{
			SetRect(sprite_batch[LWalk2][0], sprite_batch[LWalk2][1], sprite_batch[LWalk2][2], sprite_batch[LWalk2][3]);
		}
		if (direction == RIGHT)
		{
			SetRect(sprite_batch[RWalk2][0], sprite_batch[RWalk2][1], sprite_batch[RWalk2][2], sprite_batch[RWalk2][3]);
		}
	}
	else
	{
		if (direction == LEFT)
		{
			SetRect(sprite_batch[LWalk3][0], sprite_batch[LWalk3][1], sprite_batch[LWalk3][2], sprite_batch[LWalk3][3]);
		}
		if (direction == RIGHT)
		{
			SetRect(sprite_batch[RWalk3][0], sprite_batch[RWalk3][1], sprite_batch[RWalk3][2], sprite_batch[RWalk3][3]);
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
		SetRect(sprite_batch[LFall][0], sprite_batch[LFall][1], sprite_batch[LFall][2], sprite_batch[LFall][3]);
	}
	if (direction == RIGHT)
	{
		SetRect(sprite_batch[RFall][0], sprite_batch[RFall][1], sprite_batch[RFall][2], sprite_batch[RFall][3]);
	}
}

void Animation2D::SetPunch(int animation)
{
	if (animation == 1)
	{
		if (direction == LEFT)
		{
			SetRect(sprite_batch[LKick1][0], sprite_batch[LKick1][1], sprite_batch[LKick1][2], sprite_batch[LKick1][3]);
		}
		if (direction == RIGHT)
		{
			SetRect(sprite_batch[RKick1][0], sprite_batch[RKick1][1], sprite_batch[RKick1][2], sprite_batch[RKick1][3]);
		}
	}
	else if (animation == 2)
	{
		if (direction == LEFT)
		{
			SetRect(sprite_batch[LKick2][0], sprite_batch[LKick2][1], sprite_batch[LKick2][2], sprite_batch[LKick2][3]);
		}
		if (direction == RIGHT)
		{
			SetRect(sprite_batch[RKick2][0], sprite_batch[RKick2][1], sprite_batch[RKick2][2], sprite_batch[RKick2][3]);
		}
	}
	else
	{
		if (direction == LEFT)
		{
			SetRect(sprite_batch[LKick3][0], sprite_batch[LKick3][1], sprite_batch[LKick3][2], sprite_batch[LKick3][3]);
		}
		if (direction == RIGHT)
		{
			SetRect(sprite_batch[RKick3][0], sprite_batch[RKick3][1], sprite_batch[RKick3][2], sprite_batch[RKick3][3]);
		}
	}
}

void Animation2D::SetGrab()
{
	if (direction == LEFT)
	{
		SetRect(sprite_batch[LGrab][0], sprite_batch[LGrab][1], sprite_batch[LGrab][2], sprite_batch[LGrab][3]);
	}
	if (direction == RIGHT)
	{
		SetRect(sprite_batch[RGrab][0], sprite_batch[RGrab][1], sprite_batch[RGrab][2], sprite_batch[RGrab][3]);
	}
}

void Animation2D::LoadSprites(string _filename)
{
	std::ifstream sprite_position_batching;
	sprite_position_batching.open(_filename);
	if (sprite_position_batching.is_open())
	{
		while (!sprite_position_batching.eof())
		{
			for (int j = 0; j < 25; j++)
			{
				for (int i = 0; i < 4; i++) //prints into array Default Left
				{
					sprite_position_batching >> sprite_batch[j][i];
				}
			}			
		}
	}
	sprite_position_batching.close();
	SetRect(sprite_batch[LDefault][0], sprite_batch[LDefault][1], sprite_batch[LDefault][2], sprite_batch[LDefault][3]);
}