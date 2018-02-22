#include "pch.h"
#include "GameScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"


void GameScene::init(RenderData* m_RD)
{
	Player2D *m_testPlatform = new Player2D(m_RD, "gens");
	m_testPlatform->SetPos(Vector2(500, 600));
	m_testPlatform->TestCollision();
	m_2DPlatforms.push_back(m_testPlatform);

	Player2D *m_testPlatform2 = new Player2D(m_RD, "gens");
	m_testPlatform2->SetPos(Vector2(100, 300));
	m_testPlatform2->TestCollision();
	m_2DPlatforms.push_back(m_testPlatform2);

	m_player = new Player2D(m_RD, "gens");
	m_player->SetPos(Vector2(300, 300));
	m_player->SetDrive(500.0f);
	m_player->SetDrag(0.5f);
}

void GameScene::update(GameStateData* gsd)
{
	for (std::vector<GameObject2D *>::iterator it = m_2DPlatforms.begin(); it != m_2DPlatforms.end(); it++)
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

	m_RD->m_spriteBatch->End();
}

void GameScene::ReadInput(GameStateData* gsd)
{

}
