#include "pch.h"
#include "GameScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"


GameScene::~GameScene()
{
	delete[] m_player;
}

void GameScene::init(RenderData* m_RD, GameStateData* gsd)
{
	title_text = new Text2D("Super Trash Potatoes");
	title_text->SetLayer(1.0f);
	game_objects.push_back(title_text);

	no_players = 4;//gsd->no_players;
	spawnPlayers(m_RD, no_players);

	platform = new ImageGO2D(m_RD, "platform");
	platform->SetPos(Vector2(200, 600));
	platform->SetLayer(1.0f);
	platform->SetRect(1,1,600,72);
	game_objects.push_back(platform);
}

void GameScene::update(GameStateData* gsd)
{
	for (std::vector<GameObject2D *>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		for (int i = 0; i < no_players; i++)
		{
			m_player[i]->Tick(gsd, *it);
		}
	}
}

void GameScene::render(RenderData* m_RD,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);

	for (std::vector<GameObject2D *>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Render(m_RD);
	}

	for (int i = 0; i < no_players; i++)
	{
		m_player[i]->Render(m_RD);
	}

	m_RD->m_spriteBatch->End();
}

void GameScene::ReadInput(GameStateData* gsd)
{
	if ((gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape) 
		|| (gsd->m_gamePadState[0].IsStartPressed() && !gsd->m_prevGamePadState[0].IsStartPressed()))
	{
		gsd->gameState = MENU;
	}
}

void GameScene::spawnPlayers(RenderData* m_RD, int no_players)
{
	for (int i = 0; i < no_players; i++)
	{
		std::string str_player_no = "mario_sprite_batch_" + std::to_string(i);
		m_player[i] = new Player2D(m_RD, str_player_no);
		m_player[i]->SetPos(Vector2(300, 300));
		m_player[i]->SetLayer(1.0f);
		m_player[i]->SetDrive(500.0f);
		m_player[i]->SetDrag(0.5f);
		m_player[i]->LoadSprites("MarioSpriteBatch.txt");
		m_player[i]->setPlayerNo(i);
	}
}
