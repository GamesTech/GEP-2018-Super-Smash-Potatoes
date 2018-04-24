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

bool GameOverScene::init(RenderData* m_RD, GameStateData* gsd, AudioManager* am)
{
	game_over_text = std::make_unique<Text2D>("Game Ogre");
	game_over_text->SetLayer(1.0f);
	game_over_text->SetPos(Vector2(500, 360));
	game_over_text->CentreOrigin();
	game_objects.push_back(std::move(game_over_text));

	switch (gsd->winState)
	{
	case PLAYER1:
		winner_text = std::make_unique<Text2D>("PLAYER 1 WINS");
		break;
	case PLAYER2:
		winner_text = std::make_unique<Text2D>("PLAYER 2 WINS");
		break;
	case PLAYER3:
		winner_text = std::make_unique<Text2D>("PLAYER 3 WINS");
		break;
	case PLAYER4:
		winner_text = std::make_unique<Text2D>("PLAYER 4 WINS");
		break;
	default:
		winner_text = std::make_unique<Text2D>("NO WINNER");
		break;
	}
	winner_text->SetLayer(1.0f);
	winner_text->SetPos(Vector2(500, 330));
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

void GameOverScene::ReadInput(GameStateData * gsd)
{
	if ((gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape)
		|| (gsd->m_gamePadState[0].IsStartPressed() && !gsd->m_prevGamePadState[0].IsStartPressed()))
	{
		action = Action::BACK;
	}
}
