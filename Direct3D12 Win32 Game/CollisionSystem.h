#include "GameObject2D.h"
#include "Player2D.h"

class CollisionSystem
{
public:
	CollisionSystem() = default;
	~CollisionSystem() = default;
	bool ResloveCollision(GameObject2D * _obj, Player2D* _player);

private:

	bool AllSideCollision(GameObject2D * _obj, Player2D* _player);
	bool TopSideCollision(GameObject2D * _obj, Player2D* _player);
	bool CheckIntersect(GameObject2D * _obj, Player2D* _player);
};
