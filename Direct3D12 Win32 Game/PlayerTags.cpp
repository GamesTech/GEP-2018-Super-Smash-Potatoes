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
		m_player_tag[i]->SetLayer(0.3f);
		m_player_tag[i]->CentreOrigin();
	}
}

void PlayerTags::Update()
{
	for (int i = 0; i < player; ++i)
	{
		if (player_pos[i].x < 30)
		{
			player_pos[i].x = 30;
		}
		if (player_pos[i].x > 1250)
		{
			player_pos[i].x = 1250;
		}

		if (player_pos[i].y < 30)
		{
			player_pos[i].y = 30;
		}
		if (player_pos[i].y > 690)
		{
			player_pos[i].y = 690;
		}
		m_player_tag[i]->SetPos(player_pos[i]);
	}
}

void PlayerTags::Render(RenderData * m_RD)
{
	for (int i = 0; i < player; ++i)
	{
		m_player_tag[i]->Render(m_RD);
	}
}

void PlayerTags::SetPlayerPos(int player, Vector2 pos)
{
	for (int i = 0; i < player; ++i)
	{
		if (player == i)
		{
			player_pos[i] = pos;
		}
	}
}
