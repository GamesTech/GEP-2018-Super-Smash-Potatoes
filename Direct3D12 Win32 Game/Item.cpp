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
			int dir = player->GetPos().x - GetPos().x;
			if (dir > 2)dir = 2;
			if (dir < -2)dir = -2;
			player->GotHit(_GSD, dir, 1);
		}
		else {
		
		}
	}
}
