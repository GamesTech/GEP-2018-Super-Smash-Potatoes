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
	//no_players = gsd->no_players;

	title_text = std::make_unique<ImageGO2D>(m_RD, "Logo");
	title_text->SetLayer(1.0f);
	title_text->SetPos({0, 0});
	title_text->SetRect({ 0, 0, 1280, 720 });
	game_objects.push_back(std::move(title_text));

	for (int i = 0; i < 3; ++i)
	{
		button[i] = std::make_unique<ImageGO2D>(m_RD, "Buttons");
		button[i]->SetPos(button_info[i].pos);
		button[i]->SetRect(button_info[i].m_rect);
		button[i]->SetLayer(1.0f);
		button[i]->CentreOrigin();
		game_objects.push_back(std::move(button[i]));
	}

	highlight_option_selected();

	audio_manager = am;
	//audio_manager->changeLoopTrack(NIGHTAMBIENCE);

	
	particle_system.init(m_RD);

	return true;
}

Scene::SceneChange MenuScene::update(GameStateData* gsd)
{
	//Add your game logic here.
	
	for (auto& it : game_objects)
	{
		it->Tick(gsd);
	}

	particle_system.update(gsd);

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
			particle_system.spawnParticle(3, "hit", {300,300});
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
	particle_system.render(m_RD);
	m_RD->m_spriteBatch->End();
}

void MenuScene::highlight_option_selected()
{
	switch (menu_option_selected)
	{
	case 1:
		game_objects[1]->SetColour(Color(1, 0, 0));
		game_objects[2]->SetColour(Color(1, 1, 1));
		game_objects[3]->SetColour(Color(1, 1, 1));
		break;
	case 2:
		game_objects[1]->SetColour(Color(1, 1, 1));
		game_objects[2]->SetColour(Color(1, 0, 0));
		game_objects[3]->SetColour(Color(1, 1, 1));
		break;
	case 3:
		game_objects[1]->SetColour(Color(1, 1, 1));
		game_objects[2]->SetColour(Color(1, 1, 1));
		game_objects[3]->SetColour(Color(1, 0, 0));
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