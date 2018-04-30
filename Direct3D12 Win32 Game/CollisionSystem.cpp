#include "pch.h"
#include "CollisionSystem.h"

bool CollisionSystem::ResloveCollision(GameObject2D * _obj, Player2D * _player)
{
	if (_obj->GetLayer() == 0.5)
	{
		if (_obj->GetType() == 0)
		{
			return MainPlatformCollision(_obj, _player);
		}
		else
		{
			if (_player->IgnoreCollision())
			{
				return false;
			}
			return SmallPlatformCollision(_obj, _player);
		}
	}
	return false;
}

bool CollisionSystem::MainPlatformCollision(GameObject2D * _obj, Player2D* _player)
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

bool CollisionSystem::SmallPlatformCollision(GameObject2D * _obj, Player2D* _player)
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
