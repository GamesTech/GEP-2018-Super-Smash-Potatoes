#include "pch.h"
#include "PlayerTags.h"

PlayerTags::PlayerTags(int players) : player(players)
{
}

void PlayerTags::Init(RenderData * m_RD)
{
	for (int i = 0; i < player; ++i)
	{
		m_player_tag[i] = std::make_unique<ImageGO2D>(m_RD, "PlayerTags");
		m_player_tag[i]->SetPos(Vector2(250, 200));
		m_player_tag[i]->SetRect(m_player_tag_sprite[i]);
		m_player_tag[i]->SetLayer(0.0f);
		m_player_tag[i]->CentreOrigin();
	}
}

void PlayerTags::Update()
{
	for (int i = 0; i < player; ++i)
	{
		if (player_pos[i].x < 0)
		{
			player_pos[i].x = 0;
		}
		if (player_pos[i].x > 1210)
		{
			player_pos[i].x = 1210;
		}

		if (player_pos[i].y < 50)
		{
			player_pos[i].y = 50;
		}
		if (player_pos[i].y > 680)
		{
			player_pos[i].y = 680;
		}
		m_player_tag[i]->SetPos(player_pos[i] + Vector2{(width[i] /2),-25});
	}
}

void PlayerTags::Render(RenderData * m_RD)
{
	for (int i = 0; i < player; ++i)
	{
		m_player_tag[i]->Render(m_RD);
	}
}

void PlayerTags::SetPlayerPos(int players, Vector2 pos, float _width)
{
	for (int i = 0; i < player; ++i)
	{
		if (players == i)
		{
			player_pos[i] = pos;
			width[i] = _width;
		}
	}
}
