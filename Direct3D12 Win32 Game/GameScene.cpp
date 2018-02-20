#include "pch.h"
#include "GameScene.h"


GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}

void GameScene::init(RenderData* m_RD)
{
}

void GameScene::update(GameStateData* gsd)
{
	/*for (std::vector<GameObject2D *>::iterator it = m_2DPlatforms.begin(); it != m_2DPlatforms.end(); it++)
	{
		m_player->Tick(gsd, *it);
	}*/
}

void GameScene::render(RenderData* m_RD,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{

}
