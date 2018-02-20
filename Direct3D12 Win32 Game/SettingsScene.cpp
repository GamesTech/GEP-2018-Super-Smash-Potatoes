#include "pch.h"
#include "SettingsScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"

void SettingsScene::init(RenderData * m_RD)
{
	resolution_text = new Text2D("Resolution Text");
	resolution_text->SetLayer(1.0f);
	game_objects.push_back(resolution_text);

	main_menu_button = new ImageGO2D(m_RD, "Main_Menu_Button");
	main_menu_button->SetPos(Vector2(300, 300));
	main_menu_button->CentreOrigin();
}

void SettingsScene::update(GameStateData * gsd)
{
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

void SettingsScene::ReadInput(GameStateData * gsd)
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
			if (resolution_option_selected < 3)
			{
				resolution_option_selected++;
				newResolutionText(resolution_option_selected);
			}
		}
		if (gsd->m_keyboardState.Right && !gsd->m_prevKeyboardState.Right)
		{
			if (resolution_option_selected > 1)
			{
				resolution_option_selected--;
				newResolutionText(resolution_option_selected);
			}
		}
		break;
	case 2:
		if (gsd->m_keyboardState.Right && !gsd->m_prevKeyboardState.Right)
		{
			switch (resolution_option_selected)
			{
			case 1:
				//m_outputWidth = 800;
				//m_outputHeight = 600;
				break;
			case 2:
				//m_outputWidth = 1280;
				//m_outputHeight = 960;
				break;
			case 3:
				//m_outputWidth = 1440;
				//m_outputHeight = 1080;
				break;
			}
		}
	}
}


void SettingsScene::newResolutionText(int new_resolution_option)
{
	delete resolution_text;
	resolution_text = nullptr;
	switch (new_resolution_option)
	{
	case 1:
		resolution_text = new Text2D("800 x 600 ->");
		break;
	case 2:
		resolution_text = new Text2D("<- 1280 x 960 ->");
		break;
	case 3:
		resolution_text = new Text2D("<- 1440 x 1080");
		break;
	}
}

void SettingsScene::highlight_option_selected()
{
	switch (menu_option_selected)
	{
	case 1:
		resolution_text->SetColour(Color(1, 0, 0));
		main_menu_button->SetColour(Color(1, 1, 1));
		break;
	case 2:
		resolution_text->SetColour(Color(1, 1, 1));
		main_menu_button->SetColour(Color(1, 0, 0));
		break;
	}
}