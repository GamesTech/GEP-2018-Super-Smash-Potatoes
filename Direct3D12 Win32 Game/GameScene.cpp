#include "pch.h"
//#include "GameScene.h"
//#include "RenderData.h"
//#include "GameObject2D.h"
//#include "GameStateData.h"
//
//
//void GameScene::init(RenderData* m_RD)
//{
//	m_keyboard = std::make_unique<Keyboard>();
//}
//
//void GameScene::update(GameStateData* gsd)
//{
//	/*for (std::vector<GameObject2D *>::iterator it = m_2DPlatforms.begin(); it != m_2DPlatforms.end(); it++)
//	{
//		m_player->Tick(gsd, *it);
//	}*/
//}
//
//void GameScene::render(RenderData* m_RD,
//	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
//{
//	//finally draw all 2D objects
//	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
//	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
//	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);
//
//	for (std::vector<GameObject2D *>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
//	{
//		(*it)->Render(m_RD);
//	}
//
//	m_RD->m_spriteBatch->End();
//}
//
//void GameScene::ReadInput(GameStateData* gsd)
//{
//
//}
