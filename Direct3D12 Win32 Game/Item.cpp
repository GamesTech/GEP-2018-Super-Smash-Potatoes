#include "pch.h"
#include "Item.h"

Item::Item(RenderData * _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer, Type type, long life_span)
	: Physics2D(_RD, _filename, image_buffer)
{
	this->life_span = life_span;
	this->type = type;
	SetType(3);
}

void Item::update(GameStateData * _GSD)
{
	if (time >= life_span)
	{
		marked_for_deletion = true;
		time = 0;
	}
	else 
	{
		time++;
	}

	if (type == BOMB) 
	{
		if (player != nullptr)
		{
			m_pos = Vector2(player->GetPos().x + player->Width() / 2, player->GetPos().y - player->Height() / 4);
		}
	}

	if (active)
	{
		AddGravity(false);
		Tick(_GSD, false, false, 1.0f, false);
	} 
	else 
	{
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

void Item::throwItem(GameStateData* _GSD, float player_orientation)
{
	float power = 50000.0f;
	active = true;
	player = nullptr;
	AddForce(Vector2(player_orientation * power, -30000));
	Tick(_GSD, false, false, 1.0f, false);
}

bool Item::getMarked() const
{
	return marked_for_deletion;
}

void Item::collided(Player2D* _player, GameStateData *  _GSD)
{
	if (type == BOMB) 
	{
		if (active) 
		{
			if (_player->getPlayerNo() != prev_holder_id)
			{
				//float r1 = Width() * 1.5;
				float x1 = GetPos().x + (Width() / 2);
				float y1 = GetPos().y + (Height()/ 2);

				//float r2 = _player->Width();
				float x2 = _player->GetPos().x + (_player->Width() / 2);
				float y2 = _player->GetPos().y + (_player->Height() / 2);

				if (!_player->GetInvincibility())
				{
					Vector2 direction = Vector2(x2 - x1, y2 - y1);
					direction.Normalize();
					_player->GotHit(_GSD, direction.x, direction.y);
				}
				marked_for_deletion = true;
			}
		} 
		else if (player == nullptr)
		{
			if (_player->getItem() == nullptr) 
			{
				this->player = _player;
				prev_holder_id = player->getPlayerNo();
				player->setItem(this);
				m_pos = Vector2(player->GetPos().x + player->Width() / 2, player->GetPos().y - player->Height() / 4);
			}
		}
	}
	else if (type == HEAL) 
	{
		if (_player->GetDamage() > 1.0f) 
		{
			_player->SetDamage(_player->GetDamage() - 1.0f);
		}
		marked_for_deletion = true;
	}
}
