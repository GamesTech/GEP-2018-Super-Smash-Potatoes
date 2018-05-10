#include "pch.h"
#include "UserInterface2D.h"
#include "GameStateData.h"

void UserInterface::init(RenderData * m_RD, GameStateData * gsd, std::vector<std::unique_ptr<Player2D>>& players, std::vector<string> sprite_names, std::shared_ptr<ImageBuffer> image_buffer)
{
	timer_text = new Text2D("Time Remaining: xxxs");
	timer_text->SetPos(Vector2(750, 10));
	timer_text->SetLayer(1.0f);

	//player_UI_Boxes = new ImageGO2D(m_RD, "PlayerPreviewBoxes");
	//player_UI_Boxes->SetPos(Vector2(640, 640));
	//player_UI_Boxes->SetRect(1, 1, 723, 180);
	//player_UI_Boxes->SetLayer(0.1f);
	//player_UI_Boxes->CentreOrigin();
	//player_UI_Boxes->SetScale(Vector2(0.75f, 0.75f));
	//player_UI_Boxes->SetColour(DirectX::SimpleMath::Color::Color(1, 1, 1, 0.5f));
	//UI.emplace_back(player_UI_Boxes);

	for (int i = 0; i < gsd->no_players; i++)
	{
		ImageGO2D* temp_player_UI = new ImageGO2D(m_RD, sprite_names[gsd->player_selected[i]], image_buffer);
		temp_player_UI->SetPos(Vector2(410 + (i * character_gap), UI_start_y + 35));
		temp_player_UI->SetRect(1, 1, 64, 64);
		temp_player_UI->SetLayer(0.0f);
		temp_player_UI->CentreOrigin();
		UI.emplace_back(temp_player_UI);

		player_numbers = std::make_unique<ImageGO2D>(m_RD, "PlayerTags", image_buffer);
		player_numbers->SetPos(Vector2(400 + (i * character_gap), UI_start_y));
		player_numbers->SetLayer(0.0f);
		player_numbers->SetRect(number_pos[i]);
		player_numbers->CentreOrigin();
		UI.push_back(std::move(player_numbers));

		damage_text[i] = new Text2D("xxx%");
		damage_text[i]->SetPos(Vector2(435 + (i * character_gap), UI_start_y + 10));
		damage_text[i]->SetLayer(1.0f);
		damage_text[i]->CentreOrigin();
		damage_text[i]->SetColour(DirectX::SimpleMath::Color::Color(1, 1, 1, 1));
		UI.emplace_back(damage_text[i]);

		max_lives = players[0]->GetLivesRemaining();
		for (int j = 0; j < max_lives; j++)
		{
			int lives_no = (i * max_lives) + j;
			lives_button_sprite[lives_no] = new ImageGO2D(m_RD, "lives", image_buffer);
			lives_button_sprite[lives_no]->SetPos(Vector2(390 + (i * character_gap) + (j * 30), UI_start_y + 80));
			lives_button_sprite[lives_no]->SetRect(1, 1, 20, 20);
			lives_button_sprite[lives_no]->SetLayer(0.0f);
			lives_button_sprite[lives_no]->CentreOrigin();
			UI.emplace_back(lives_button_sprite[lives_no]);
		}
	}
}

void UserInterface::update(GameStateData* gsd, std::vector<std::unique_ptr<Player2D>> &players, float time_remaining)
{
	time_remaining = time_remaining - gsd->m_dt;
	timer_text->SetText("Time Remaining: " + std::to_string(time_remaining) + "s");

	for (auto& player : players)
	{
		int player_damage = (player->GetDamage() * 100) - 100;

		damage_text[player->getPlayerNo()]->SetText(std::to_string(player_damage) + "%");

		for (int j = 0; j < max_lives - (player->GetLivesRemaining()); j++)
		{
			lives_button_sprite[(player->getPlayerNo() * 3) + j]->SetColour(DirectX::SimpleMath::Color(1, 0, 0));
		}
	}
}

void UserInterface::render(RenderData * m_RD)
{
	for (auto& UI : UI)
	{
		UI->Render(m_RD);
	}
}


