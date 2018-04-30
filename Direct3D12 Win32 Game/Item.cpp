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
		if(player != nullptr)m_pos = player->GetPos() - Vector2(0, yOffset);
	}
}

void Item::collided(Player2D* player, GameStateData *  _GSD)
{
	if (type == BOMB) {
		if (active) {
			int dir = player->GetPos().x - GetPos().x;
			if (dir > 4)dir = 4;
			if (dir < -4)dir = -4;
			player->GotHit(_GSD, dir, 1.5);
		}
		else {
			this->player = player;
			m_pos = player->GetPos() - Vector2(0, yOffset);
		}
	}
}
