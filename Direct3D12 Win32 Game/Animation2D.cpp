#include "pch.h"
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

void Animation2D::SetRun()
{
}

void Animation2D::SetFall()
{
}

void Animation2D::SetPunch()
{
}

void Animation2D::loadSprites(string _filename)
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
			for (int i = 0; i < 4; i++) //prints into array Jump Left
			{
				sprite_position_batching >> left_walk_positions[i];
			}
			for (int i = 0; i < 4; i++) //prints into array Jump Right
			{
				sprite_position_batching >> right_walk_positions[i];
			}
		}
	}
	sprite_position_batching.close();
}
