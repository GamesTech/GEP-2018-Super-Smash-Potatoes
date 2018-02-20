#include "pch.h"
#include "MenuScene.h"
#include "RenderData.h"
#include "GameObject2D.h"


MenuScene::MenuScene()
{
}


MenuScene::~MenuScene()
{
}

void MenuScene::init(RenderData* m_RD)
{
	title_text = new Text2D("Super Trash Potatoes");
	title_text->SetLayer(1.0f);
	game_objects.push_back(title_text);

	start_game_button = new ImageGO2D(m_RD, "Start_Game_Button");
	start_game_button->SetPos(Vector2(300, 200));
	start_game_button->SetLayer(1.0f);
	start_game_button->CentreOrigin();
	game_objects.push_back(start_game_button);

	settings_button = new ImageGO2D(m_RD, "Settings_Button");
	settings_button->SetPos(Vector2(300, 300));
	settings_button->CentreOrigin();
	game_objects.push_back(settings_button);

	quit_button = new ImageGO2D(m_RD, "Quit_Button");
	quit_button->SetPos(Vector2(300, 400));
	quit_button->CentreOrigin();
	game_objects.push_back(quit_button);
}

void MenuScene::update(GameStateData* gsd)
{
	//Add your game logic here.

	for (std::vector<GameObject2D *>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Tick(gsd);
	}
}

void MenuScene::render(RenderData* m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
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

void MenuScene::highlight_menu_option()
{
	switch (menu_option_selected)
	{
	case 1:
		start_game_button->SetColour(Color(1, 0, 0));
		settings_button->SetColour(Color(1, 1, 1));
		quit_button->SetColour(Color(1, 1, 1));
		break;
	case 2:
		start_game_button->SetColour(Color(1, 1, 1));
		settings_button->SetColour(Color(1, 0, 0));
		quit_button->SetColour(Color(1, 1, 1));
		break;
	case 3:
		start_game_button->SetColour(Color(1, 1, 1));
		settings_button->SetColour(Color(1, 1, 1));
		quit_button->SetColour(Color(1, 0, 0));
		break;
	default:
		//something has gone wrong
		break;
	}
}
