#include "pch.h"
#include "Item.h"

Item::Item(RenderData * _RD, string _filename, Type type, long life_span)
	: Physics2D(_RD, _filename)
{
	this->life_span = life_span;
	this->type = type;
}

void Item::update()
{
	if (time >= life_span)
	{
		marked_for_deletion = true;
		time = 0;
	}
	else {
		time++;
	}

	if (type == BOMB) {
		if (player != nullptr)
			m_pos = player->GetPos() - Vector2(0, yOffset);
	}
}

void Item::resetPlayer()
{
	player = nullptr;
}

void Item::setActive(bool active)
{
	this->active = active;
}

void Item::setMarked(bool marked)
{
	marked_for_deletion = marked;
}

void Item::setLife(long life)
{
	life_span = life;
}

bool Item::getMarked() const
{
	return marked_for_deletion;
}

void Item::collided(Player2D* player, GameStateData *  _GSD)
{
	if (type == BOMB) {
		if (active) {
			int dir = player->GetPos().x - GetPos().x;
			if (dir < 32 && dir >= 0)dir = 0;
			else if (dir > -32 && dir <= 0)dir = 0;
			else if (dir > 0) dir = 1;
			else if (dir < 0)dir = -1;
			player->GotHit(_GSD, dir, 1.5);
			// todo: Spawn Effect Explosion
			// todo: Play sound Explosion
			marked_for_deletion = true;
		}
		else {
			if (player->getItem() == nullptr) 
			{
				this->player = player;
				player->setItem(this);
				m_pos = player->GetPos() - Vector2(0, yOffset);
			}
		}
	}
}
