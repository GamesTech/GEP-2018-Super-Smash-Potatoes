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
	for (auto object : game_objects)
	{
		if (object)
		{
			delete object;
			object = nullptr;
		}
	}
	game_objects.clear();
}


void CharacterSelectScene::init(RenderData * m_RD, GameStateData * gsd)
{
	loadCharactersFile("PlayerSprites.txt");

	title_text = new Text2D("Character Select!");
	title_text->SetLayer(1.0f);
	game_objects.push_back(title_text);

	for (int i = 0; i < sprite_names.size(); i++)
	{
		selection_option[i] = new ImageGO2D(m_RD, sprite_names[i]);
		selection_option[i]->SetPos(Vector2((i * 200) + 200, 200));
		selection_option[i]->SetRect(1, 1, 200, 200);
		selection_option[i]->SetLayer(1.0f);
		selection_option[i]->CentreOrigin();
		game_objects.push_back(selection_option[i]);
	}



	player_preview_boxes = new ImageGO2D(m_RD, "PlayerPreviewBoxes");
	player_preview_boxes->SetPos(Vector2(640, 620));
	player_preview_boxes->SetRect(1, 1, 723, 180);
	player_preview_boxes->SetLayer(1.0f);
	player_preview_boxes->CentreOrigin();
	game_objects.push_back(player_preview_boxes);

	player1_preview = new ImageGO2D(m_RD, "gens2");
	player1_preview->SetPos(Vector2(640-270, 620));
	player1_preview->SetRect(1, 1, 100, 100);
	player1_preview->SetLayer(1.0f);
	player1_preview->CentreOrigin();
	game_objects.push_back(player1_preview);

	player2_preview = new ImageGO2D(m_RD, "gens2");
	player2_preview->SetPos(Vector2(640-90, 620));
	player2_preview->SetRect(1, 1, 100, 100);
	player2_preview->SetLayer(1.0f);
	player2_preview->CentreOrigin();
	game_objects.push_back(player2_preview);

	player3_preview = new ImageGO2D(m_RD, "gens2");
	player3_preview->SetPos(Vector2(640+90, 620));
	player3_preview->SetRect(1, 1, 100, 100);
	player3_preview->SetLayer(1.0f);
	player3_preview->CentreOrigin();
	game_objects.push_back(player3_preview);

	player4_preview = new ImageGO2D(m_RD, "gens2");
	player4_preview->SetPos(Vector2(640+270, 620));
	player4_preview->SetRect(1, 1, 100, 100);
	player4_preview->SetLayer(1.0f);
	player4_preview->CentreOrigin();
	game_objects.push_back(player4_preview);

}

void CharacterSelectScene::update(GameStateData * gsd)
{
	for (std::vector<GameObject2D *>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Tick(gsd);
	}
}

void CharacterSelectScene::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);

	for (std::vector<GameObject2D *>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Render(m_RD);
	}

	m_RD->m_spriteBatch->End();
}



void CharacterSelectScene::ReadInput(GameStateData * gsd)
{
	for (int i = 0; i < gsd->no_players; i++)
	{
		if ((gsd->m_keyboardState.Up && !gsd->m_prevKeyboardState.Up)
			|| (gsd->m_gamePadState[i].IsDPadUpPressed() && !gsd->m_prevGamePadState[0].IsDPadUpPressed()))
		{

		}
		if ((gsd->m_keyboardState.Down && !gsd->m_prevKeyboardState.Down)
			|| (gsd->m_gamePadState[i].IsDPadDownPressed() && !gsd->m_prevGamePadState[0].IsDPadDownPressed()))
		{

		}
		if ((gsd->m_keyboardState.Left && !gsd->m_prevKeyboardState.Left)
			|| (gsd->m_gamePadState[i].IsDPadLeftPressed() && !gsd->m_prevGamePadState[0].IsDPadLeftPressed()))
		{
			selection_player[i]--;
		}
		if ((gsd->m_keyboardState.Right && !gsd->m_prevKeyboardState.Right)
			|| (gsd->m_gamePadState[i].IsDPadRightPressed() && !gsd->m_prevGamePadState[0].IsDPadRightPressed()))
		{
			selection_player[i]++;
		}

		if ((gsd->m_keyboardState.Enter && !gsd->m_prevKeyboardState.Enter)
			|| (gsd->m_gamePadState[i].IsAPressed() && !gsd->m_prevGamePadState[0].IsAPressed()))
		{

		}
	}
	if (gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape)
	{
		gsd->gameState = MENU;
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