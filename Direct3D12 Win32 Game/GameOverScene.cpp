#include "pch.h"
#include "GameOverScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
	game_objects.clear();
}

bool GameOverScene::init(RenderData* _RD, GameStateData* gsd, AudioManager* am)
{
	m_RD = _RD;
	winner_number = std::make_unique<ImageGO2D>(m_RD, "numbers");
	winner_number->SetLayer(1.0f);
	winner_number->SetPos(Vector2(700, 330));
	winner_number->SetRect(number_pos[gsd->winState]);
	winner_number->CentreOrigin();
	game_objects.push_back(std::move(winner_number));
	
	winner_text = std::make_unique<ImageGO2D>(m_RD, "Player wins");
	winner_text->SetLayer(1.0f);
	winner_text->SetRect(1,1,924,60);
	winner_text->SetPos(Vector2(640, 330));
	winner_text->CentreOrigin();
	game_objects.push_back(std::move(winner_text));
	return true;
}

Scene::SceneChange GameOverScene::update(GameStateData * gsd)
{
	Scene::SceneChange scene_change;
	switch (action)
	{
	case Action::BACK:
	{
		scene_change.change_type = ChangeType::REPLACE_ALL;
		m_RD->m_resourceCount = 1;
		scene_change.scene = SceneEnum::MENU;
		break;
	}
	}
	action = Action::NONE;
	return scene_change;
}

void GameOverScene::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);

	for (auto& it : game_objects)
	{
		it->Render(m_RD);
	}

	m_RD->m_spriteBatch->End();
}

void GameOverScene::ReadInput(Input * input_manager)
{
	if (input_manager->inputs[0] == START)
	{
		action = Action::BACK;
	}
}
