#include "pch.h"
#include "CollisionSystem.h"

bool CollisionSystem::ResloveCollision(GameObject2D * _obj, Player2D * _player)
{
	if (_obj->GetLayer() == 0.5)
	{
		if (_obj->GetType() == 0)
		{
			return AllSideCollision(_obj, _player);
		}
		else if (_obj->GetType() == 3)
		{
			return CheckIntersect(_obj, _player);
		}
		else
		{
			if (_player->IgnoreCollision())
			{
				return false;
			}
			return TopSideCollision(_obj, _player);
		}
	}
	return false;
}

bool CollisionSystem::AllSideCollision(GameObject2D * _obj, Player2D* _player)
{
	Player2D* player = _player;
	GameObject2D* object = _obj;
	
	float width = 0.5 * (player->Width() + object->Width());
	float height = 0.5 * (player->Height() + object->Height());
	float distance_x = player->CenterX() - object->CenterX();
	float distance_y = player->CenterY() - object->CenterY();

	if (abs(distance_x) <= width && abs(distance_y) <= height)
	{
		// collision occured

		float collision_width = width * distance_y;
		float collision_height = height * distance_x;

		if (collision_width < collision_height)
		{
			if (collision_width < -collision_height)
			{
				if (player->GetCurrVel().y >= 0)
				{
					player->SetNewPos(object->GetPos().y - player->Height());
					player->SetCollState(player->COLTOP);
					return true;
					// at the top 
				}
				else
				{
					player->SetCollState(player->COLNONE);
					return false;
				}
			}
			else if (collision_width > -collision_height)
			{
				if (!player->GetLedgeJump())
				{
					player->SetNewPos(object->GetPos().x + object->Width());
					player->SetCollState(player->COLRIGHT);
					return true;
					// on the right 
				}
				else
				{
					player->SetCollState(player->COLNONE);
					return false;
				}
			}

		}
		else if (collision_width > collision_height)
		{
			if (collision_width > -collision_height)
			{
				if (player->GetCurrVel().y < 0)
				{
					player->SetNewPos(object->GetPos().y + object->Height());
					player->SetCollState(player->COLBOTTOM);
					return true;
					// collision at the bottom 
				}
				else
				{
					player->SetCollState(player->COLNONE);
					return false;
				}
			}
			else if (collision_width < -collision_height)
			{
				if (!player->GetLedgeJump())
				{
					player->SetNewPos(object->GetPos().x - player->Width());
					player->SetCollState(player->COLLEFT);
					return true;
					// on the left 
				}
				else
				{
					player->SetCollState(player->COLNONE);
					return false;
				}
			}
		}
	}
	else
	{
		player->SetCollState(player->COLNONE);
		return false;
	}
}

bool CollisionSystem::TopSideCollision(GameObject2D * _obj, Player2D* _player)
{
	Player2D* player = _player;
	GameObject2D* object = _obj;

	float width = 0.5 * (player->Width() + object->Width());
	float height = 0.5 * (player->Height() + object->Height());
	float distance_x = player->CenterX() - object->CenterX();
	float distance_y = player->CenterY() - object->CenterY();

	if (abs(distance_x) <= width && abs(distance_y) <= height)
	{
		// collision occured

		float collision_width = width * distance_y;
		float collision_height = height * distance_x;

		if (collision_width < collision_height)
		{
			if (collision_width < -collision_height)
			{
				if (player->GetCurrVel().y >= 0)
				{
					player->SetNewPos(object->GetPos().y - player->Height());
					player->SetCollState(player->COLTOP);
					return true;
					// at the top 
				}
			}
		}
	}
	
	player->SetCollState(player->COLNONE);
	return false;
}

bool CollisionSystem::CheckIntersect(GameObject2D * _obj, Player2D * _player, float _r1, float _r1_multiplier, float _x_offset, float _y_offset, int _direction)
{
	float r1 = 0;

	if (_r1 == 0)
	{
		r1 = _obj->Width() * _r1_multiplier;
	}
	else
	{
		r1 = _r1 * _r1_multiplier;
	}

	float x1 = _obj->GetPos().x + (_obj->Width() / 2);
	float y1 = _obj->GetPos().y + (_obj->Height() / 2);
	
	switch (_direction)
	{
	case 0:
		x1 += _x_offset;
		y1 += _y_offset;
		break;
	case -1:
		x1 -= _x_offset;
		break;
	case 1:
		x1 += _x_offset;
		break;
	default:
		break;
	}

	float r2 = _player->Width();
	float x2 = _player->GetPos().x + (_player->Width() / 2);
	float y2 = _player->GetPos().y + (_player->Height() / 2);

	if (r1 > sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1))))
	{
		return  true;
	}
	return false;
}
