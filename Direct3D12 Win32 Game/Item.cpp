#include "pch.h"
#include "Item.h"

Item::Item(RenderData * _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer, Type type, long life_span)
	: Physics2D(_RD, _filename, image_buffer)
{
	this->life_span = life_span;
	this->type = type;
}

void Item::update(GameStateData * _GSD)
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

	if (active)
	{
		AddGravity(false);
		Tick(_GSD, false, false, 1.0f, false);
	} else {
		m_vel = Vector2(0, 0);
		m_acc = Vector2(0, 0);
	}
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

void Item::throwItem(GameStateData* _GSD, Vector2 player_movement)
{
	float power = 250.0f;
	active = true;
	player = nullptr;
	AddForce(player_movement * power);
	Tick(_GSD, false, false, 1.0f, false);
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
			if (dir < 16 && dir >= 0)dir = 0;
			else if (dir > -16 && dir <= 0)dir = 0;
			else if (dir > 0) dir = 1;
			else if (dir < 0)dir = -1;
			player->GotHit(_GSD, dir, 1.5);
			// todo: Spawn Effect Explosion
			// todo: Play sound Explosion
			marked_for_deletion = true;
		} else {
			if (player->getItem() == nullptr) 
			{
				this->player = player;
				player->setItem(this);
				m_pos = player->GetPos() - Vector2(0, yOffset);
			}
		}
	}
	else if (type == HEAL) {
		if (player->GetDamage() > 1.0f) {
			player->SetDamage(player->GetDamage() - 1.0f);
		}
		marked_for_deletion = true;
	}
}
