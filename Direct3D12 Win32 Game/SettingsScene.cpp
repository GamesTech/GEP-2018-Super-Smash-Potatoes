#include "pch.h"
#include "SettingsScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"

SettingsScene::~SettingsScene()
{

}

bool SettingsScene::init(RenderData* m_RD, GameStateData* gsd, AudioManager* am)
{
	no_players = gsd->no_players;

	resolution_text = std::make_unique<Text2D>("Resolution Text");
	resolution_text->SetPos(Vector2(300, 200));
	resolution_text->SetLayer(1.0f);
	newResolutionText(3);
	resolution_option_selected = 3;

	fullscreen_text = std::make_unique<Text2D>("Fullscreen: False");
	fullscreen_text->SetPos(Vector2(300, 300));
	fullscreen_text->SetLayer(1.0f);

	main_menu_button = std::make_unique<ImageGO2D>(m_RD, "Buttons");
	main_menu_button->SetPos(Vector2(300, 400));
	main_menu_button->SetRect(1, 241, 240, 320);
	game_objects.push_back(std::move(main_menu_button));

	highlight_option_selected();
	return true;
}

Scene::SceneChange SettingsScene::update(GameStateData * gsd)
{
	for (auto& it : game_objects)
	{
		it->Tick(gsd);
	}

	switch (resolution_option_selected)
	{
	case 1:
		new_outputWidth = 800;
		new_outputHeight = 600;
		break;
	case 2:
		new_outputWidth = 1024;
		new_outputHeight = 768;
		break;
	case 3:
		new_outputWidth = 1280;
		new_outputHeight = 720;
		break;
	case 4:
		new_outputWidth = 1920;
		new_outputHeight = 1080;
		break;
	}

	DXGI_MODE_DESC newTargetParams = { new_outputWidth, new_outputHeight, 60, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED };
	m_swapChain->ResizeTarget(&newTargetParams);

	Scene::SceneChange scene_change;
	switch (action)
	{
		case Action::FULLSCREEN:
		{
			if (fullscreen)
			{
				fullscreen_text->SetText("Fullscreen: True");
				m_swapChain->SetFullscreenState(true, NULL);
			}
			else
			{
				fullscreen_text->SetText("Fullscreen: False");
				m_swapChain->SetFullscreenState(false, NULL);
			}
			break;
		}

		case Action::SCREEN_RES_LEFT:
		{
			if (resolution_option_selected > 1)
			{
				resolution_option_selected--;
				newResolutionText(resolution_option_selected);
			}
			break;
		}

		case Action::SCREEN_RES_RIGHT:
		{
			if (resolution_option_selected < 4)
			{
				resolution_option_selected++;
				newResolutionText(resolution_option_selected);
			}
			break;
		}

		case Action::BUTTON_UP:
		{
			if (menu_option_selected > 1)
			{
				menu_option_selected--;
				highlight_option_selected();
			}
			break;
		}

		case Action::BUTTON_DOWN:
		{
			if (menu_option_selected < 3)
			{
				menu_option_selected++;
				highlight_option_selected();
			}
			break;
		}

		case Action::BUTTON_PRESSED:
		{
			scene_change.change_type = ChangeType::REMOVE;
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

void SettingsScene::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);

	for (auto& it : game_objects)
	{
		it->Render(m_RD);
	}
	resolution_text->Render(m_RD);
	fullscreen_text->Render(m_RD);

	m_RD->m_spriteBatch->End();
}

void SettingsScene::ReadInput(Input* input_manager)
{
	for (int i = 0; i < no_players; i++)
	{
		if (input_manager->inputs[i] == DOWN)
		{
			action = Action::BUTTON_DOWN;
		}

		else if (input_manager->inputs[i] == UP)
		{
			action = Action::BUTTON_UP;
		}

		switch (menu_option_selected)
		{
		case 1:
			if (input_manager->inputs[i] == LEFT)
			{
				action = Action::SCREEN_RES_LEFT;
			}
			else if (input_manager->inputs[i] == RIGHT)
			{
				action = Action::SCREEN_RES_RIGHT;
			}
			break;
		case 2:
			if (input_manager->inputs[i] == LEFT)
			{
				fullscreen = false;
				action = Action::FULLSCREEN;
			}
			else if (input_manager->inputs[i] == RIGHT)
			{
				fullscreen = true;
				action = Action::FULLSCREEN;
			}
			break;
		case 3:
			if (input_manager->inputs[i] == A)
			{
				action = Action::EXIT;
			}
			break;
		}
	}
	input_manager->clearInput();
}


void SettingsScene::newResolutionText(int new_resolution_option)
{
	switch (new_resolution_option)
	{
	case 1:
		resolution_text->SetText("Resolution: 800 x 600 ->");
		break;
	case 2:
		resolution_text->SetText("Resolution: <- 1024 x 768 ->"); //Arcade Machine Reso
		break;
	case 3:
		resolution_text->SetText("Resolution: <- 1280 x 720 ->");
		break;
	case 4:
		resolution_text->SetText("Resolution: <- 1920 x 1080");
		break;
	}
}

void SettingsScene::highlight_option_selected()
{
	switch (menu_option_selected)
	{
	case 1:
		resolution_text->SetColour(Color(1, 0, 0));
		fullscreen_text->SetColour(Color(1, 1, 1));
		game_objects[0]->SetColour(Color(1, 1, 1));
		break;
	case 2:
		resolution_text->SetColour(Color(1, 1, 1));
		fullscreen_text->SetColour(Color(1, 0, 0));
		game_objects[0]->SetColour(Color(1, 1, 1));
		break;
	case 3:
		resolution_text->SetColour(Color(1, 1, 1));
		fullscreen_text->SetColour(Color(1, 1, 1));
		game_objects[0]->SetColour(Color(1, 0, 0));
		break;
	}
}

void SettingsScene::giveSwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain)
{
	m_swapChain = swapChain;
}
