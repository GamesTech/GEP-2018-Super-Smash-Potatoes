#include "pch.h"
#include "PauseScene.h"


bool PauseScene::init(RenderData * m_RD, GameStateData * gsd, AudioManager * am, std::shared_ptr<ImageBuffer> ib)
{
	image_buffer = ib;
	game_objects.emplace_back(new Text2D("PAUSE"));
	game_objects.back()->SetPos(Vector2(640, 300));
	game_objects.back()->SetLayer(0.f);
	game_objects.back()->CentreOrigin();
	return true;
}

Scene::SceneChange PauseScene::update(GameStateData * gsd)
{
	Scene::SceneChange scene_change;
	switch (action)
	{
	case Action::EXIT:
	{
		scene_change.change_type = ChangeType::REMOVE;
		break;
	}
	}
	action = Action::NONE;
	return scene_change;
}

void PauseScene::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);

	for (auto& it : game_objects)
	{
		it->Render(m_RD);
	}
	m_RD->m_spriteBatch->End();
}

void PauseScene::ReadInput(Input * input_manager)
{
	input_manager->current_scene = CurrentScene::START;
	for (int i = 0; i < 4; ++i)
	{
		if (input_manager->inputs[i] == Inputs::START || input_manager->inputs[i] == Inputs::ESCAPE)
		{
			action = Action::EXIT;
		}
	}
}