#include "pch.h"
#include "MenuScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"
#include "LevelFile.h"


MenuScene::~MenuScene()
{
	game_objects.clear();
}

bool MenuScene::init(RenderData* m_RD, GameStateData* gsd, AudioManager* am)
{
	title_text = std::make_unique<Text2D>("Super Trash Potatoes");
	title_text->SetLayer(1.0f);
	game_objects.push_back(std::move(title_text));

	start_game_button = std::make_unique<ImageGO2D>(m_RD, "Buttons");
	start_game_button->SetPos(Vector2(300, 200));
	start_game_button->SetRect(1, 1, 240, 80);
	start_game_button->SetLayer(1.0f);
	start_game_button->CentreOrigin();
	game_objects.push_back(std::move(start_game_button));

	settings_button = std::make_unique<ImageGO2D>(m_RD, "Buttons");
	settings_button->SetPos(Vector2(300, 300));
	settings_button->SetRect(1, 81, 240, 160);
	settings_button->CentreOrigin();
	game_objects.push_back(std::move(settings_button));

	quit_button = std::make_unique<ImageGO2D>(m_RD, "Buttons");
	quit_button->SetPos(Vector2(300, 400));
	quit_button->SetRect(1, 161, 240, 240);
	quit_button->CentreOrigin();
	game_objects.push_back(std::move(quit_button));

	highlight_option_selected();

	audio_manager = am;
	//audio_manager->changeLoopTrack(NIGHTAMBIENCE);
	return true;
}

Scene::SceneChange MenuScene::update(GameStateData* gsd)
{
	//Add your game logic here.
	
	for (auto& it : game_objects)
	{
		it->Tick(gsd);
	}

	Scene::SceneChange scene_change;
	switch (action)
	{
		case Action::BUTTON_UP:
		{
			if (menu_option_selected > 1)
			{
				menu_option_selected--;
				highlight_option_selected();
				audio_manager->playSound(TOBYMENUCLICK1);
			}
			break;
		}

		case Action::BUTTON_DOWN:
		{
			if (menu_option_selected < 3)
			{
				menu_option_selected++;
				highlight_option_selected();
				audio_manager->playSound(TOBYMENUCLICK1);
			}
			break;
		}

		case Action::BUTTON_PRESSED:
		{
			switch (menu_option_selected)
			{
			case 1:
			{
				scene_change.change_type = ChangeType::ADD;
				scene_change.scene = SceneEnum::CHARACTER_SELECTION;
				break;
			}
			case 2:
			{
				scene_change.change_type = ChangeType::ADD;
				scene_change.scene = SceneEnum::SETTINGS;
				break;
			}
			case 3:
			{
				scene_change.change_type = ChangeType::REMOVE;
				break;
			}
			}
			
		}
	}
	action = Action::NONE;
	return scene_change;

}

void MenuScene::render(RenderData* m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
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

void MenuScene::highlight_option_selected()
{
	switch (menu_option_selected)
	{
	case 1:
		//start_game_button->SetColour(Color(1, 0, 0));
		//settings_button->SetColour(Color(1, 1, 1));
		//quit_button->SetColour(Color(1, 1, 1));
		break;
	case 2:
		//start_game_button->SetColour(Color(1, 1, 1));
		//settings_button->SetColour(Color(1, 0, 0));
		//quit_button->SetColour(Color(1, 1, 1));
		break;
	case 3:
		//start_game_button->SetColour(Color(1, 1, 1));
		//settings_button->SetColour(Color(1, 1, 1));
		//quit_button->SetColour(Color(1, 0, 0));
		break;
	}
}

void MenuScene::ReadInput(GameStateData* gsd)
{
	if ((gsd->m_keyboardState.Down && !gsd->m_prevKeyboardState.Down) 
		|| (gsd->m_gamePadState[0].IsDPadDownPressed() && !gsd->m_prevGamePadState[0].IsDPadDownPressed()))
	{
		action = Action::BUTTON_DOWN;
	}
	if ((gsd->m_keyboardState.Up && !gsd->m_prevKeyboardState.Up)
		|| (gsd->m_gamePadState[0].IsDPadUpPressed() && !gsd->m_prevGamePadState[0].IsDPadUpPressed()))
	{
		action = Action::BUTTON_UP;
	}

	if ((gsd->m_keyboardState.Enter && !gsd->m_prevKeyboardState.Enter)
		|| (gsd->m_gamePadState[0].IsAPressed() && !gsd->m_prevGamePadState[0].IsAPressed()))
	{
		action = Action::BUTTON_PRESSED;
	}
}