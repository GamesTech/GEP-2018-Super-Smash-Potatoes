#include "pch.h"
#include "StartScene.h"


bool StartScene::init(RenderData * m_RD, GameStateData * gsd, AudioManager * am, std::shared_ptr<ImageBuffer> ib)
{
	viewport = { -1.f, -1.f,
		static_cast<float>(1280), static_cast<float>(720),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };

	m_RD->m_spriteBatch->SetViewport(viewport);
	image_buffer = ib;
	game_objects.emplace_back(new Text2D("Press any button to start"));
	game_objects.back()->SetPos(Vector2(300, 300));
	game_objects.back()->SetLayer(0.f);
	
	loadBackground(m_RD, ib);
	return true;
}

Scene::SceneChange StartScene::update(GameStateData * gsd)
{
	Scene::SceneChange scene_change;
	switch (action)
	{
		case Action::START:
		{
			scene_change.change_type = ChangeType::ADD;
			scene_change.scene = SceneEnum::MENU;
			break;
		}

		case Action::EXIT:
		{
			scene_change.change_type = ChangeType::REMOVE;
			break;
		}
	}
	action = Action::NONE;
	return scene_change;
}

void StartScene::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
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

void StartScene::ReadInput(Input * input_manager)
{
	input_manager->current_scene = CurrentScene::START;
	for (int i = 0; i < 4; ++i)
	{
		if (input_manager->inputs[i] == Inputs::B)
		{
			action = Action::EXIT;
		}
		else if (input_manager->inputs[i] != Inputs::CLEAR)
		{
			action = Action::START;
		}
	}
}

