#include "pch.h"
#include "ArenaSelectScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"

ArenaSelectScene::ArenaSelectScene()
{
}

ArenaSelectScene::~ArenaSelectScene()
{
	for (auto object : game_objects)
	{
		if (object)
		{
			delete object;
			object = nullptr;
		}
	}

	platforms.clear();
	game_objects.clear();
}

void ArenaSelectScene::init(RenderData * m_RD, GameStateData * gsd)
{
	title_text = new Text2D("Arena Select!");
	title_text->SetLayer(1.0f);
	game_objects.push_back(title_text);
	
	level_name_text = new Text2D("Level X");
	level_name_text->SetLayer(1.0f);
	level_name_text->SetPos(Vector2(500, 50));
	game_objects.push_back(level_name_text);

	left_arrow = new ImageGO2D(m_RD, "Arrow");
	left_arrow->SetLayer(1.0f);
	left_arrow->SetRect(1, 1, 260, 200);
	left_arrow->SetPos(Vector2(180, 360));
	left_arrow->CentreOrigin();
	game_objects.push_back(left_arrow);

	right_arrow = new ImageGO2D(m_RD, "Arrow");
	right_arrow->SetLayer(1.0f);
	right_arrow->SetRect(1, 1, 260, 200);
	right_arrow->SetPos(Vector2(1100, 360));
	right_arrow->CentreOrigin();
	right_arrow->SetOrientation(3.14f);
	game_objects.push_back(right_arrow);

	loadLevelsFile("Levels.txt");
	loadLevel(m_RD, level_names[0]);
}

void ArenaSelectScene::update(GameStateData * gsd)
{

}

void ArenaSelectScene::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	if (new_level == true)
	{
		new_level = false;
		loadLevel(m_RD, level_names[level_selected]);
	}

	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);

	for (std::vector<GameObject2D *>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Render(m_RD);
	}
	for (auto& platform : platforms)
	{
		platform->Render(m_RD);
	}
	m_RD->m_spriteBatch->End();
}

void ArenaSelectScene::ReadInput(GameStateData * gsd)
{
	if ((gsd->m_keyboardState.Left && !gsd->m_prevKeyboardState.Left)
		|| (gsd->m_gamePadState[0].IsDPadLeftPressed() && !gsd->m_prevGamePadState[0].IsDPadLeftPressed()))
	{
		if (level_selected > 0)
		{
			new_level = true;
			level_selected--;
		}
	}
	if ((gsd->m_keyboardState.Right && !gsd->m_prevKeyboardState.Right)
		|| (gsd->m_gamePadState[0].IsDPadRightPressed() && !gsd->m_prevGamePadState[0].IsDPadRightPressed()))
	{
		if (level_selected < total_levels-1)
		{
			new_level = true;
			level_selected++;
		}
	}

	if ((gsd->m_keyboardState.Enter && !gsd->m_prevKeyboardState.Enter)
		|| (gsd->m_gamePadState[0].IsStartPressed() && !gsd->m_prevGamePadState[0].IsStartPressed()))
	{
		gsd->arena_selected = level_selected;
		gsd->gameState = INGAME;
	}

	if ((gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape)
		|| (gsd->m_gamePadState[0].IsBPressed() && !gsd->m_prevGamePadState[0].IsBPressed()))
	{
		gsd->gameState = CHARACTERSELECT;
	}
}

void ArenaSelectScene::loadLevel(RenderData* m_RD, string lvlname)
{
	platforms.clear();

	level = std::make_unique<LevelFile>();
	level->read(lvlname, ".lvl");

	level_name_text->SetText(lvlname);

	for (int i = 0; i < level->getObjListSize(); i++)
	{
		string temp_name = level->getObj(i).image_file;
		auto platform = new ImageGO2D(m_RD, temp_name);

		platform->SetPos(level->getObj(i).position);
		platform->SetOrigin(level->getObj(i).origin);
		platform->SetScale(level->getObj(i).scale);
		platform->SetOri(level->getObj(i).orientation);
		platform->SetLayer(level->getObj(i).layer);

		platform->SetRect(level->getObj(i).sprite_size_min.x, level->getObj(i).sprite_size_min.y, level->getObj(i).sprite_size_max.x, level->getObj(i).sprite_size_max.y);
		platforms.emplace_back(platform);
	}
}


void ArenaSelectScene::loadLevelsFile(string _filename)
{
	std::ifstream level_loading;
	level_loading.open(_filename);
	if (level_loading.is_open())
	{
		while (!level_loading.eof())
		{
			std::string temp_string;
			level_loading >> temp_string;
			level_names.push_back(temp_string);
			total_levels++;
		}
	}
	level_loading.close();
}