#include "pch.h"
#include "GameScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"


void GameScene::init(RenderData* m_RD)
{
	title_text = new Text2D("Super Trash Potatoes");
	title_text->SetLayer(1.0f);
	game_objects.push_back(title_text);

	m_player = new Player2D(m_RD, "mario_sprite_batch");
	m_player->SetPos(Vector2(300, 300));
	m_player->SetLayer(1.0f);
	m_player->SetDrive(500.0f);
	m_player->SetDrag(0.5f);
	m_player->loadSprites("MarioSpriteBatch.txt");
	m_player->SetRect(724, 0, 775, 63);
	//game_objects.push_back(m_player);

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
		m_player->Tick(gsd, *it);
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

	m_player->Render(m_RD);
	m_RD->m_spriteBatch->End();
}

void GameScene::ReadInput(GameStateData* gsd)
{
	if (gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape)
	{
		gsd->gameState = MENU;
	}

}
