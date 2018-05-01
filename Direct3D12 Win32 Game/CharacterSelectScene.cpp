#include "pch.h"
#include "CharacterSelectScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"

#include<iostream>
#include<fstream>

CharacterSelectScene::CharacterSelectScene()
{
}

CharacterSelectScene::~CharacterSelectScene()
{
	sprite_names.clear();
	grid_sprites.clear();
	player_previews.clear();
	game_objects.clear();
	selection_numbers.clear();
}


bool CharacterSelectScene::init(RenderData* m_RD, GameStateData* gsd, AudioManager* am)
{
	no_players = gsd->no_players;
	loadCharactersFile("PlayerSprites.txt");

	title_text = std::make_unique<ImageGO2D>(m_RD, "Character Selection");
	title_text->SetLayer(1.0f);
	title_text->SetRect(1,1,1280,720);
	game_objects.push_back(std::move(title_text));

	for (int i = 0; i < gsd->MAX_PLAYERS; i++)
	{
		player_numbers = std::make_unique<ImageGO2D>(m_RD, "PlayerTags");
		player_numbers->SetLayer(0.0f);
		player_numbers->SetRect(number_pos[i]);
		player_numbers->CentreOrigin();
		selection_numbers.push_back(std::move(player_numbers));
	}

	sprites_per_row = 9;
	int current_sprites_on_row = 0;
	int row_no = 0;

	for (int i = 0; i < sprite_names.size(); i++)
	{
		grid_sprite_temp = std::make_unique<ImageGO2D>(m_RD, sprite_names[i]);
		grid_sprite_temp->SetPos(Vector2(sprite_pixel_gap + (current_sprites_on_row * sprite_pixel_gap), sprite_pixel_gap + (row_no * sprite_pixel_gap)));
		grid_sprite_temp->SetRect(1, 1, grid_sprite_pixel_size, grid_sprite_pixel_size);
		grid_sprite_temp->SetLayer(1.0f);
		grid_sprite_temp->CentreOrigin();
		grid_sprite_temp->SetScale(Vector2{ 2,2 });
		grid_sprites.push_back(std::move(grid_sprite_temp));

		player_preview_temp = std::make_unique<ImageGO2D>(m_RD, sprite_names[i]);
		player_preview_temp->SetRect(1, 1, grid_sprite_pixel_size, grid_sprite_pixel_size);
		player_preview_temp->SetLayer(0.0f);
		player_preview_temp->CentreOrigin();
		player_preview_temp->SetScale(Vector2{2.25,2.25});
		player_previews.push_back(std::move(player_preview_temp));

		//new row
		current_sprites_on_row++;

		if (current_sprites_on_row == sprites_per_row)
		{
			row_no++;
			current_sprites_on_row = 0;
		}
	}

	player_preview_boxes = std::make_unique<ImageGO2D>(m_RD, "PlayerPreviewBoxes");
	player_preview_boxes->SetPos(Vector2(640, 600));
	player_preview_boxes->SetRect(1, 1, 723, 180);
	player_preview_boxes->SetLayer(1.0f);
	player_preview_boxes->CentreOrigin();
	player_preview_boxes->SetScale(Vector2{ 1.02, 1 });
	game_objects.push_back(std::move(player_preview_boxes));

	return true;

}

Scene::SceneChange CharacterSelectScene::update(GameStateData * gsd)
{
	no_players = gsd->no_players;

	int players_ready = 0;
	for (int i = 0; i < no_players; i++)
	{
		if (players_locked[i] == true)
		{
			players_ready++;
		}
	}

	for (auto& it : game_objects)
	{
		it->Tick(gsd);
	}

	//all players ready, next scene
	if (no_players == players_ready)
	{
		for (int i = 0; i < no_players; i++)
		{
			gsd->player_selected[i] = selection_player[i];
		}
		action = Action::CONTINUE;
	}
	Scene::SceneChange scene_change;
	switch (action)
	{
	case Action::CONTINUE:
	{
		scene_change.change_type = ChangeType::ADD;
		scene_change.scene = SceneEnum::ARENA_SELECTION;
		for (int i = 0; i < no_players; i++)
		{
			players_locked[i] = false;
		}
		break;
	}

	case Action::BACK:
	{
		scene_change.change_type = ChangeType::REMOVE;
		break;
	}
	}
	action = Action::NONE;
	return scene_change;
}

void CharacterSelectScene::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);

	for (auto& it : game_objects)
	{
		it->Render(m_RD);
	}
	for (auto& it : grid_sprites)
	{
		it->Render(m_RD);
	}

	for (int i = 0; i < no_players; i++)
	{
		player_previews[selection_player[i]]->SetPos(Vector2(352 + (i * 184), 607));
		if (players_locked[i] == true)
		{
			player_previews[selection_player[i]]->SetColour(DirectX::SimpleMath::Color::Color(0, 1, 0));
		}
		else
		{
			player_previews[selection_player[i]]->SetColour(DirectX::SimpleMath::Color::Color(1, 1, 1));
		}
		player_previews[selection_player[i]]->Render(m_RD);

		//numbers!
		selection_numbers[i]->SetPos((grid_sprites[selection_player[i]]->GetPos()) - (Vector2(grid_sprite_pixel_size - (i * (sprite_pixel_gap / 3.5)), sprite_pixel_gap - grid_sprite_pixel_size)));
		selection_numbers[i]->Render(m_RD);
	}

	m_RD->m_spriteBatch->End();
}



void CharacterSelectScene::ReadInput(GameStateData * gsd)
{
	for (int i = 0; i < no_players; i++)
	{
		if (players_locked[i] == false)
		{
			if ((gsd->m_keyboardState.Up && !gsd->m_prevKeyboardState.Up)
				|| (gsd->m_gamePadState[i].IsDPadUpPressed() && !gsd->m_prevGamePadState[i].IsDPadUpPressed()))
			{
				grid_sprites[selection_player[i]]->SetColour(DirectX::SimpleMath::Color::Color(1, 1, 1));
				grid_sprites[selection_player[i]]->SetScale(Vector2(2.0f, 2.0f));
				grid_sprites[selection_player[i]]->SetLayer(1.0f);

				if ((selection_player[i] - sprites_per_row) >= 0)
				{
					selection_player[i] = selection_player[i] - sprites_per_row;
				}
			}
			if ((gsd->m_keyboardState.Down && !gsd->m_prevKeyboardState.Down)
				|| (gsd->m_gamePadState[i].IsDPadDownPressed() && !gsd->m_prevGamePadState[i].IsDPadDownPressed()))
			{
				grid_sprites[selection_player[i]]->SetColour(DirectX::SimpleMath::Color::Color(1, 1, 1));
				grid_sprites[selection_player[i]]->SetScale(Vector2(2.0f, 2.0f));
				grid_sprites[selection_player[i]]->SetLayer(1.0f);

				if ((selection_player[i] + sprites_per_row) < grid_sprites.size())
				{
					selection_player[i] = selection_player[i] + sprites_per_row;
				}
			}
			if ((gsd->m_keyboardState.Left && !gsd->m_prevKeyboardState.Left)
				|| (gsd->m_gamePadState[i].IsDPadLeftPressed() && !gsd->m_prevGamePadState[i].IsDPadLeftPressed()))
			{
				grid_sprites[selection_player[i]]->SetColour(DirectX::SimpleMath::Color::Color(1, 1, 1));
				grid_sprites[selection_player[i]]->SetScale(Vector2(2.0f, 2.0f));
				grid_sprites[selection_player[i]]->SetLayer(1.0f);

				if ((selection_player[i] - 1) >= 0)
				{
					selection_player[i]--;
				}
			}
			if ((gsd->m_keyboardState.Right && !gsd->m_prevKeyboardState.Right)
				|| (gsd->m_gamePadState[i].IsDPadRightPressed() && !gsd->m_prevGamePadState[i].IsDPadRightPressed()))
			{
				grid_sprites[selection_player[i]]->SetColour(DirectX::SimpleMath::Color::Color(1, 1, 1));
				grid_sprites[selection_player[i]]->SetScale(Vector2(2.0f, 2.0f));
				grid_sprites[selection_player[i]]->SetLayer(1.0f);

				if ((selection_player[i] + 1) < grid_sprites.size())
				{
					selection_player[i]++;
				}
			}
		}

		/*
		don't know if we'll want this back?
		float red = 0;
		float green = 0;
		float blue = 0;
		switch (i)
		{
		case 0:
			red = 1;
			break;
		case 1:
			blue = 1;
			break;
		case 2:
			green = 1;
			break;
		case 3:
			red = 0.5f;
			green = 0.5f;
			break;
		}

		grid_sprites[selection_player[i]]->SetColour(DirectX::SimpleMath::Color::Color(red, green, blue));
		*/

		grid_sprites[selection_player[i]]->SetScale(Vector2(2.2f, 2.2f));
		grid_sprites[selection_player[i]]->SetLayer(0.9f);
		//player_previews[selection_player[i]]->SetPos(Vector2(370 + (i * 180), 620));

		if ((gsd->m_keyboardState.Enter && !gsd->m_prevKeyboardState.Enter)
			|| (gsd->m_gamePadState[i].IsAPressed() && !gsd->m_prevGamePadState[i].IsAPressed()))
		{
			players_locked[i] = !players_locked[i];
		}
	}

	//only player1
	if ((gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape)
		|| (gsd->m_gamePadState[0].IsBPressed() && !gsd->m_prevGamePadState[0].IsBPressed()))
	{
		action = Action::BACK;
	}
}

void CharacterSelectScene::loadCharactersFile(string _filename)
{
	std::ifstream character_sprites_loading;
	character_sprites_loading.open(_filename);
	if (character_sprites_loading.is_open())
	{
		while (!character_sprites_loading.eof())
		{
			std::string temp_string;
			character_sprites_loading >> temp_string;
			sprite_names.push_back(temp_string);
		}
	}
		character_sprites_loading.close();
}