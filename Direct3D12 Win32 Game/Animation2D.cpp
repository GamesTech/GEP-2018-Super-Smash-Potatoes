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
			switch (m_change_animation)
			{
				case 1:
				{
					SetWalk();
					break;
				}
				case 2:
				{
					SetWalk1();
					break;
				}
				case 3:
				{
					SetWalk2();
					break;
				}
			}
		}
		else if (action_movement == GRAB)
		{
			SetGrab();
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
	else if (action_jump == FALL)
	{
		SetFall();
	}
	
	
	if (timer >= 0.3)
	{
		if (m_change_animation < 3)
		{
			m_change_animation++;
		}
		else
		{
			m_change_animation = 1;
		}
		timer -= 0.3;
	}
	else
	{
		timer += _GSD->m_dt;
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
			SetRect(left_default_positions[0], left_default_positions[1], left_default_positions[2], left_default_positions[3]);
		}
		if (direction == RIGHT)
		{
			SetRect(right_default_positions[0], right_default_positions[1], right_default_positions[2], right_default_positions[3]);
		}
	}
}

void Animation2D::SetJump()
{
	if (direction == LEFT)
	{
		SetRect(left_jump_positions[0], left_jump_positions[1], left_jump_positions[2], left_jump_positions[3]);
	}
	if (direction == RIGHT)
	{
		SetRect(right_jump_positions[0], right_jump_positions[1], right_jump_positions[2], right_jump_positions[3]);
	}
}

void Animation2D::SetWalk()
{
	if (direction == LEFT)
	{
		SetRect(left_walk_positions[0], left_walk_positions[1], left_walk_positions[2], left_walk_positions[3]);
	}
	if (direction == RIGHT)
	{
		SetRect(right_walk_positions[0], right_walk_positions[1], right_walk_positions[2], right_walk_positions[3]);
	}
}

void Animation2D::SetWalk1()
{
	if (direction == LEFT)
	{
		SetRect(left_walk_1_positions[0], left_walk_1_positions[1], left_walk_1_positions[2], left_walk_1_positions[3]);
	}
	if (direction == RIGHT)
	{
		SetRect(right_walk_1_positions[0], right_walk_1_positions[1], right_walk_1_positions[2], right_walk_1_positions[3]);
	}
}

void Animation2D::SetWalk2()
{
	if (direction == LEFT)
	{
		SetRect(left_walk_2_positions[0], left_walk_2_positions[1], left_walk_2_positions[2], left_walk_2_positions[3]);
	}
	if (direction == RIGHT)
	{
		SetRect(right_walk_2_positions[0], right_walk_2_positions[1], right_walk_2_positions[2], right_walk_2_positions[3]);
	}
}

void Animation2D::SetRun()
{
}

void Animation2D::SetFall()
{
	SetRect(fall_positions[0], fall_positions[1], fall_positions[2], fall_positions[3]);
}

void Animation2D::SetPunch()
{
}

void Animation2D::SetGrab()
{
	if (direction == LEFT)
	{
		SetRect(left_grab_positions[0], left_grab_positions[1], left_grab_positions[2], left_grab_positions[3]);
	}
	if (direction == RIGHT)
	{
		SetRect(right_grab_positions[0], right_grab_positions[1], right_grab_positions[2], right_grab_positions[3]);
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
			for (int i = 0; i < 4; i++) //prints into array Default Left
			{
				sprite_position_batching >> left_default_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Default Right
			{
				sprite_position_batching >> right_default_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Jump Left
			{
				sprite_position_batching >> left_jump_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Jump Right
			{
				sprite_position_batching >> right_jump_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Walk Left
			{
				sprite_position_batching >> left_walk_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Walk Right
			{
				sprite_position_batching >> right_walk_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Walk Left
			{
				sprite_position_batching >> left_walk_1_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Walk Right
			{
				sprite_position_batching >> right_walk_1_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Walk Left
			{
				sprite_position_batching >> left_walk_2_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Walk Right
			{
				sprite_position_batching >> right_walk_2_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Falling
			{
				sprite_position_batching >> fall_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Falling
			{
				sprite_position_batching >> left_grab_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Falling
			{
				sprite_position_batching >> right_grab_positions[i];
			}
		}
	}
	sprite_position_batching.close();
	SetRect(left_default_positions[0], left_default_positions[1], left_default_positions[2], left_default_positions[3]);
}