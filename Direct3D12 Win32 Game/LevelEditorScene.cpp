#include "pch.h"
#include "LevelEditorScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"
#include "LevelFile.h"

LevelEditor::~LevelEditor()
{
}

bool LevelEditor::init(RenderData * _RD, GameStateData * gsd, AudioManager * am, std::shared_ptr<ImageBuffer> ib)
{
	image_buffer = ib;
	m_RD = _RD;
	viewport = { -0.675f, -0.7f,
		static_cast<float>(1920), static_cast<float>(1080),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	UI_viewport = { -1.f, -1.f,
		static_cast<float>(1280), static_cast<float>(720),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };

	ui = std::make_unique<ImageGO2D>(_RD, "Editor_UI", ib);
	ui->SetLayer(0.f);
	ui->SetPos(Vector2{-300, -150});
	ui->SetRect(0,0,1920,1080);
	deathzone = std::make_unique<ImageGO2D>(_RD, "Deathzone", ib);
	deathzone->SetLayer(0.1f);
	deathzone->SetPos(Vector2{ -312, -162 });
	deathzone->SetRect(0, 0, 1920, 1080);
	loadBackgrounds();

	return true;
}

Scene::SceneChange LevelEditor::update(GameStateData * gsd)
{
	for (auto& object : platforms)
	{
		object->Tick(gsd);
	}

	// Action switch
	Scene::SceneChange scene_change;
	switch (action)
	{
		case Action::ADD_BLOCK:
		{
			createNewObject(object_type);
			break;
		}
		case Action::B:
		{
			if (platforms.size() != 0)
			{
				platforms.pop_back();
			}
			break;
		}
		case Action::Y:
		{
			if (backgrounds.size() != 0)
			{
				if (background >= backgrounds.size() - 1)
				{
					background = 0;
				}
				else
				{
					background++;
				}
			}
			break;
		}
		case Action::BUTTON_UP:
		{
			moveLastObject(0, -50);
			break;
		}
		case Action::BUTTON_DOWN:
		{
			moveLastObject(0, 50);
			break;
		}
		case Action::BUTTON_LEFT:
		{
			moveLastObject(-50, 0);
			break;
		}
		case Action::BUTTON_RIGHT:
		{
			moveLastObject(50, 0);
			break;
		}
		case Action::LB:
		{
			if (platforms.size() != 0)
			{
				if (object_type != 0)
				{
					object_type--;
					platforms.pop_back();
					createNewObject(object_type);
				}
			}
			break;
		}
		case Action::RB:
		{
			if (platforms.size() != 0)
			{
				if (object_type != 3)
				{
					object_type++;
					platforms.pop_back();
					createNewObject(object_type);
				}
			}
			break;
		}
		case Action::PAUSE:
		{
			if (platforms.size() != 0)
			{
				saveLevel();
			}
		}
		case Action::BACK:
		{
			scene_change.change_type = ChangeType::REMOVE;
			viewport = { -1.f, -1.f,
				static_cast<float>(1280), static_cast<float>(720),
				D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			m_RD->m_spriteBatch->SetViewport(viewport);
			break;
		}
	}
	action = Action::NONE;
	return scene_change;
}

void LevelEditor::moveLastObject(float x, float y)
{
	if (!platforms.size() == 0)
	{
		if ((platforms.back()->GetPos().x + x) >= -250 && 
			(platforms.back()->GetPos().y + y) >= -100 &&
			(platforms.back()->GetPos().x + x) <= 1550 - platforms.back()->Width() &&
			(platforms.back()->GetPos().y + y) <= 870 - platforms.back()->Height())
		{
			platforms.back()->SetPos(platforms.back()->GetPos() + Vector2{ x, y });
		}
	}
}


void LevelEditor::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);
	//Renders the background
	m_RD->m_spriteBatch->SetViewport(UI_viewport);
	backgrounds.at(background)->Render(m_RD);
	m_RD->m_spriteBatch->End();

	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);
	m_RD->m_spriteBatch->SetViewport(viewport);
	for (auto& object : platforms)
	{
		object->Render(m_RD);
	}
	ui->Render(m_RD);
	deathzone->Render(m_RD);
	m_RD->m_spriteBatch->End();
}

void LevelEditor::ReadInput(GameStateData * gsd)
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
	if ((gsd->m_keyboardState.Left && !gsd->m_prevKeyboardState.Left)
		|| (gsd->m_gamePadState[0].IsDPadLeftPressed() && !gsd->m_prevGamePadState[0].IsDPadLeftPressed()))
	{
		action = Action::BUTTON_LEFT;
	}
	if ((gsd->m_keyboardState.Right && !gsd->m_prevKeyboardState.Right)
		|| (gsd->m_gamePadState[0].IsDPadRightPressed() && !gsd->m_prevGamePadState[0].IsDPadRightPressed()))
	{
		action = Action::BUTTON_RIGHT;
	}

	if ((gsd->m_keyboardState.Q && !gsd->m_prevKeyboardState.Q)
		|| (gsd->m_gamePadState[0].IsLeftShoulderPressed() && !gsd->m_prevGamePadState[0].IsLeftShoulderPressed()))
	{
		action = Action::LB;
	}
	if ((gsd->m_keyboardState.E && !gsd->m_prevKeyboardState.E)
		|| (gsd->m_gamePadState[0].IsRightShoulderPressed() && !gsd->m_prevGamePadState[0].IsRightShoulderPressed()))
	{
		action = Action::RB;
	}

	if ((gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape)
		|| (gsd->m_gamePadState[0].IsStartPressed() && !gsd->m_prevGamePadState[0].IsStartPressed()))
	{
		action = Action::PAUSE;
	}

	if ((gsd->m_keyboardState.Enter && !gsd->m_prevKeyboardState.Enter)
		|| (gsd->m_gamePadState[0].IsAPressed() && !gsd->m_prevGamePadState[0].IsAPressed()))
	{
		action = Action::ADD_BLOCK;
	}
	if ((gsd->m_keyboardState.Back && !gsd->m_prevKeyboardState.Back)
		|| (gsd->m_gamePadState[0].IsBPressed() && !gsd->m_prevGamePadState[0].IsBPressed()))
	{
		action = Action::B;
	}
	if ((gsd->m_keyboardState.R && !gsd->m_prevKeyboardState.R)
		|| (gsd->m_gamePadState[0].IsYPressed() && !gsd->m_prevGamePadState[0].IsYPressed()))
	{
		action = Action::Y;
	}
}

void LevelEditor::createNewObject(int type) //load a new object from the default objects .objs file.
{
	level = std::make_unique<LevelFile>();
	level->read("default_objects", ".objs");

	string temp_name = level->getObj(type).image_file;
	auto platform = new ImageGO2D(m_RD, temp_name, image_buffer);

	platform->SetPos(level->getObj(type).position);
	platform->SetOrigin(level->getObj(type).origin);
	platform->SetScale(level->getObj(type).scale);
	platform->SetOri(level->getObj(type).orientation);
	platform->SetLayer(level->getObj(type).layer);
	platform->SetType(level->getObj(type).type);
	platform->SetRect(level->getObj(type).sprite_size_min.x, level->getObj(type).sprite_size_min.y, level->getObj(type).sprite_size_max.x, level->getObj(type).sprite_size_max.y);
	platforms.emplace_back(platform);
}

void LevelEditor::loadBackgrounds()
{
	level = std::make_unique<LevelFile>();
	level->read("backgrounds", ".objs");
	for (int i = 0; i < level->getObjListSize(); i++)
	{
		string temp_name = level->getObj(i).image_file;
		backgrounds.emplace_back(new ImageGO2D(m_RD, temp_name, image_buffer));
		backgrounds.back()->SetPos(level->getObj(i).position);
		backgrounds.back()->SetOrigin(level->getObj(i).origin);
		backgrounds.back()->SetScale(level->getObj(i).scale);
		backgrounds.back()->SetOri(level->getObj(i).orientation);
		backgrounds.back()->SetLayer(level->getObj(i).layer);
		backgrounds.back()->SetType(level->getObj(i).type);
		backgrounds.back()->SetRect(level->getObj(i).sprite_size_min.x, level->getObj(i).sprite_size_min.y, level->getObj(i).sprite_size_max.x, level->getObj(i).sprite_size_max.y);
	}
}

void LevelEditor::saveLevel()
{
	if (backgrounds.size() != 0)
	{
		GameObjectData obj;
		obj.image_file = backgrounds.at(background)->getImageName();
		obj.position = backgrounds.at(background)->GetPos();
		obj.origin = backgrounds.at(background)->GetOrigin();
		obj.scale = backgrounds.at(background)->GetScale();
		obj.orientation = backgrounds.at(background)->GetOri();
		obj.layer = backgrounds.at(background)->GetLayer();
		obj.type = backgrounds.at(background)->GetType();
		obj.sprite_size_min = Vector2{ float(backgrounds.at(background)->getRect().left), float(backgrounds.at(background)->getRect().top) };
		obj.sprite_size_max = Vector2{ float(backgrounds.at(background)->getRect().right), float(backgrounds.at(background)->getRect().bottom) };
		level->objectToWrite(obj);
	}
	for (auto& platform : platforms)
	{
		GameObjectData obj;
		obj.image_file = platform->getImageName();
		obj.position = platform->GetPos();
		obj.origin = platform->GetOrigin();
		obj.scale = platform->GetScale();
		obj.orientation = platform->GetOri();
		obj.layer = platform->GetLayer();
		obj.type = platform->GetType();
		obj.sprite_size_min = Vector2{ float(platform->getRect().left), float(platform->getRect().top) };
		obj.sprite_size_max = Vector2{ float(platform->getRect().right), float(platform->getRect().bottom) };
		level->objectToWrite(obj);
	}
	level->write("level" + std::to_string(3), ".lvl");
}