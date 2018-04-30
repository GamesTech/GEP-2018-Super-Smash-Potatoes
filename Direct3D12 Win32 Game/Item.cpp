#include "pch.h"
#include "Item.h"

Item::Item(RenderData * _RD, string _filename, Type type)
	: ImageGO2D(_RD, _filename)
{
	this->type = type;
}

void Item::update()
{
	if (type == BOMB) {
		
	}
}

void Item::collided(Player2D* player, GameStateData *  _GSD)
{
	if (type == BOMB) {
		if (active) {
			player->GotHit(_GSD, 4, 1);
			//player->AddForce(Vector2(1000000, 1000));
		}
		else {
		
		}
	}
}
