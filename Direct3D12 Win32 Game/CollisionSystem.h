#include "GameObject2D.h"
#include "Player2D.h"

class CollisionSystem
{
public:
	CollisionSystem() = default;
	~CollisionSystem() = default;
	bool ResloveCollision(GameObject2D * _obj, Player2D* _player);

private:

	bool MainPlatformCollision(GameObject2D * _obj, Player2D* _player);
	bool SmallPlatformCollision(GameObject2D * _obj, Player2D* _player);
};
