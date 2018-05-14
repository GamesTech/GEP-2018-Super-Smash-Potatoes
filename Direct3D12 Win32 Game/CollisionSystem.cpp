#include "pch.h"
#include "CollisionSystem.h"

// automatically execute the right collision code based by the object type 
bool CollisionSystem::ResloveCollision(GameObject2D * _obj, Physics2D * _physics_obj)
{
	if (_obj->GetLayer() == 0.5)
	{
		if (_obj->GetType() == 0)
		{
			return AllSideCollision(_obj, _physics_obj);
		}
		else if (_obj->GetType() == 3)
		{
			return CheckIntersect(_obj, _physics_obj);
		}
		else
		{
			if (_physics_obj->IgnoreCollision())
			{
				return false;
			}
			return TopSideCollision(_obj, _physics_obj);
		}
	}
	return false;
}

// check collision with side detection
bool CollisionSystem::AllSideCollision(GameObject2D * _obj, Physics2D * _physics_obj)
{
	Physics2D* physics_obj = _physics_obj;
	GameObject2D* object = _obj;
	
	float width = 0.5 * (physics_obj->Width() + object->Width());
	float height = 0.5 * (physics_obj->Height() + object->Height());
	float distance_x = physics_obj->CenterX() - object->CenterX();
	float distance_y = physics_obj->CenterY() - object->CenterY();

	if (abs(distance_x) <= width && abs(distance_y) <= height)
	{
		// collision occured
		float collision_width = width * distance_y;
		float collision_height = height * distance_x;

		if (collision_width < collision_height)
		{
			if (collision_width < -collision_height)
			{
				// if player is falling or stationary - collision at the top
				if (physics_obj->GetVel().y >= 0)
				{
					physics_obj->SetNewPos(object->GetPos().y - physics_obj->Height());
					physics_obj->SetCollState(physics_obj->COLTOP);
					return true;
					// at the top 
				}
				else
				{
					physics_obj->SetCollState(physics_obj->COLNONE);
					return false;
				}
			}
			else if (collision_width > -collision_height)
			{
				if (!physics_obj->GetLedgeJump())
				{
					physics_obj->SetNewPos(object->GetPos().x + object->Width());
					physics_obj->SetCollState(physics_obj->COLRIGHT);
					return true;
					// on the right 
				}
				else
				{
					physics_obj->SetCollState(physics_obj->COLNONE);
					return false;
				}
			}

		}
		else if (collision_width > collision_height)
		{
			if (collision_width > -collision_height)
			{
				// if player is jumping up
				if (physics_obj->GetVel().y < 0)
				{
					physics_obj->SetNewPos(object->GetPos().y + object->Height());
					physics_obj->SetCollState(physics_obj->COLBOTTOM);
					return true;
					// collision at the bottom 
				}
				else
				{
					physics_obj->SetCollState(physics_obj->COLNONE);
					return false;
				}
			}
			else if (collision_width < -collision_height)
			{
				if (!physics_obj->GetLedgeJump())
				{
					physics_obj->SetNewPos(object->GetPos().x - physics_obj->Width());
					physics_obj->SetCollState(physics_obj->COLLEFT);
					return true;
					// on the left 
				}
				else
				{
					physics_obj->SetCollState(physics_obj->COLNONE);
					return false;
				}
			}
		}
	}
	else
	{
		physics_obj->SetCollState(physics_obj->COLNONE);
		return false;
	}
}

// check collision just at the top
bool CollisionSystem::TopSideCollision(GameObject2D * _obj, Physics2D * _physics_obj)
{
	Physics2D* physics_obj = _physics_obj;
	GameObject2D* object = _obj;

	float width = 0.5 * (physics_obj->Width() + object->Width());
	float height = 0.5 * (physics_obj->Height() + object->Height());
	float distance_x = physics_obj->CenterX() - object->CenterX();
	float distance_y = physics_obj->CenterY() - object->CenterY();

	if (abs(distance_x) <= width && abs(distance_y) <= height)
	{
		// collision occured
		float collision_width = width * distance_y;
		float collision_height = height * distance_x;

		if (collision_width < collision_height)
		{
			if (collision_width < -collision_height)
			{
				if (physics_obj->GetVel().y >= 0)
				{
					physics_obj->SetNewPos(object->GetPos().y - physics_obj->Height());
					physics_obj->SetCollState(physics_obj->COLTOP);
					return true;
					// at the top 
				}
			}
		}
	}
	
	physics_obj->SetCollState(physics_obj->COLNONE);
	return false;
}

// check intersect with an object
bool CollisionSystem::CheckIntersect(GameObject2D * _obj, Physics2D * _physics_obj,
									float _r1, float _r1_multiplier, 
									float _x_offset, float _y_offset, int _direction)
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

	float r2 = _physics_obj->Width();
	float x2 = _physics_obj->GetPos().x + (_physics_obj->Width() / 2);
	float y2 = _physics_obj->GetPos().y + (_physics_obj->Height() / 2);

	if (r1 > sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1))))
	{
		return  true;
	}
	return false;
}
