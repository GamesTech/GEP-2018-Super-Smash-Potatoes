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
	game_objects.clear();
}

void ArenaSelectScene::init(RenderData * m_RD, GameStateData * gsd)
{
	title_text = new Text2D("Arena Select!");
	title_text->SetLayer(1.0f);
	game_objects.push_back(title_text);

	loadLevelsFile("Levels.txt");
	loadLevel(m_RD, level_names[0]);

	int four = 4;
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
	if ((gsd->m_keyboardState.Space && !gsd->m_prevKeyboardState.Space)
		|| (gsd->m_gamePadState[0].IsAPressed() && !gsd->m_prevGamePadState[0].IsAPressed()))
	{
		new_level = true;
		level_selected++;
	}

	if ((gsd->m_keyboardState.Enter && !gsd->m_prevKeyboardState.Enter)
		|| (gsd->m_gamePadState[0].IsStartPressed() && !gsd->m_prevGamePadState[0].IsStartPressed()))
	{
		gsd->gameState = INGAME;
	}
}

void ArenaSelectScene::loadLevel(RenderData* m_RD, string lvlname)
{
	platforms.clear();

	level = std::make_unique<LevelFile>();
	level->read(lvlname, ".lvl");

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
		}
	}
	level_loading.close();
}