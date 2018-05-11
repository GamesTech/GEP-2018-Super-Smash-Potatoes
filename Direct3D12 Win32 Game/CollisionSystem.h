#include "GameObject2D.h"
#include "Player2D.h"

class CollisionSystem
{
public:
	CollisionSystem() = default;
	~CollisionSystem() = default;
	bool ResloveCollision(GameObject2D * _obj, Player2D* _player);
	bool CheckIntersect(GameObject2D * _obj, Player2D * _player, float _r1 = 0, float _r_multiplier = 1.2f, float _x_offset = 0.f, float _y_offset = 0.f, int _direction = 0);

private:

	bool AllSideCollision(GameObject2D * _obj, Player2D* _player);
	bool TopSideCollision(GameObject2D * _obj, Player2D* _player);
};
