#include "pch.h"
#include "SettingsScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"

SettingsScene::~SettingsScene()
{
	if (resolution_text)
	{
		delete(resolution_text);
		resolution_text = nullptr;
	}
	if (main_menu_button)
	{
		delete main_menu_button;
		main_menu_button = nullptr;
	}
}

void SettingsScene::init(RenderData * m_RD)
{
	resolution_text = new Text2D("Resolution Text");
	resolution_text->SetPos(Vector2(300, 200));
	resolution_text->SetLayer(1.0f);
	game_objects.push_back(resolution_text);
	newResolutionText(2);
	resolution_option_selected = 2;

	fullscreen_text = new Text2D("Fullscreen: False");
	fullscreen_text->SetPos(Vector2(300, 300));
	fullscreen_text->SetLayer(1.0f);
	game_objects.push_back(fullscreen_text);

	main_menu_button = new ImageGO2D(m_RD, "Main_Menu_Button");
	main_menu_button->SetPos(Vector2(300, 400));
	main_menu_button->SetRect(1, 1, 240, 80);
	game_objects.push_back(main_menu_button);

	highlight_option_selected();
}

void SettingsScene::update(GameStateData * gsd)
{
	for (std::vector<GameObject2D *>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Tick(gsd);
	}
}

void SettingsScene::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
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

void SettingsScene::ReadInput(GameStateData* gsd)
{
	if (gsd->m_keyboardState.Down && !gsd->m_prevKeyboardState.Down)
	{
		if (menu_option_selected < 3)
		{
			menu_option_selected++;
			highlight_option_selected();
		}
	}
	if (gsd->m_keyboardState.Up && !gsd->m_prevKeyboardState.Up)
	{
		if (menu_option_selected > 1)
		{
			menu_option_selected--;
			highlight_option_selected();
		}
	}

	switch (menu_option_selected)
	{
	case 1:
		if (gsd->m_keyboardState.Left && !gsd->m_prevKeyboardState.Left)
		{
			if (resolution_option_selected > 1)
			{
				resolution_option_selected--;
				newResolutionText(resolution_option_selected);
			}
		}
		if (gsd->m_keyboardState.Right && !gsd->m_prevKeyboardState.Right)
		{
			if (resolution_option_selected < 4)
			{
				resolution_option_selected++;
				newResolutionText(resolution_option_selected);
			}
		}
		break;
	case 2:
		if (gsd->m_keyboardState.Left && !gsd->m_prevKeyboardState.Left && fullscreen == true)
		{
			fullscreen = false;
			fullscreen_text->SetText("Fullscreen: False");
			m_swapChain->SetFullscreenState(false, NULL);
		}
		if (gsd->m_keyboardState.Right && !gsd->m_prevKeyboardState.Right && fullscreen == false)
		{
			fullscreen = true;
			fullscreen_text->SetText("Fullscreen: True");
			m_swapChain->SetFullscreenState(true, NULL);
		}
	case 3:
		if (gsd->m_keyboardState.Enter && !gsd->m_prevKeyboardState.Enter)
		{
			gsd->gameState = MENU;
		}
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
		new_outputWidth = 1440;
		new_outputHeight = 1080;
		break;
	}

	DXGI_MODE_DESC newTargetParams = {new_outputWidth, new_outputHeight, 60, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED };
	m_swapChain->ResizeTarget(&newTargetParams);
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
		resolution_text->SetText("Resolution: <- 1440 x 1080");
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
		main_menu_button->SetColour(Color(1, 1, 1));
		break;
	case 2:
		resolution_text->SetColour(Color(1, 1, 1));
		fullscreen_text->SetColour(Color(1, 0, 0));
		main_menu_button->SetColour(Color(1, 1, 1));
		break;
	case 3:
		resolution_text->SetColour(Color(1, 1, 1));
		fullscreen_text->SetColour(Color(1, 1, 1));
		main_menu_button->SetColour(Color(1, 0, 0));
		break;
	}
}

void SettingsScene::giveSwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain)
{
	m_swapChain = swapChain;
}
