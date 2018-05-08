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
	platforms.clear();
	game_objects.clear();
}

bool ArenaSelectScene::init(RenderData* m_RD, GameStateData* gsd, AudioManager* am, std::shared_ptr<ImageBuffer> ib)
{
	game_state_data = gsd;
	image_buffer = ib;
	no_players = gsd->no_players;
	title_boarder = std::make_unique<ImageGO2D>(m_RD, "Arena Selection", image_buffer);
	title_boarder->SetLayer(0.0f);
	title_boarder->SetRect(1, 1, 1280, 720);
	game_objects.push_back(std::move(title_boarder));

	left_arrow = std::make_unique<ImageGO2D>(m_RD, "Arrow", image_buffer);
	left_arrow->SetLayer(0.1f);
	left_arrow->SetRect(1, 1, 260, 200);
	left_arrow->SetPos(Vector2(180, 360));
	left_arrow->CentreOrigin();
	game_objects.push_back(std::move(left_arrow));

	right_arrow = std::make_unique<ImageGO2D>(m_RD, "Arrow", image_buffer);
	right_arrow->SetLayer(0.1f);
	right_arrow->SetRect(1, 1, 260, 200);
	right_arrow->SetPos(Vector2(1100, 360));
	right_arrow->CentreOrigin();
	right_arrow->SetOrientation(3.14f);
	game_objects.push_back(std::move(right_arrow));

	loadLevelsFile("Levels.txt");
	loadLevel(m_RD, level_names[0]);
	return true;
}

Scene::SceneChange ArenaSelectScene::update(GameStateData * gsd)
{
	Scene::SceneChange scene_change;
	switch (action)
	{
	case Action::CONTINUE:
	{
		scene_change.change_type = ChangeType::ADD;
		scene_change.scene = SceneEnum::GAME;
		break;
	}

	case Action::BACK:
	{
		scene_change.change_type = ChangeType::REMOVE;
		break;
	}
	}
	action = Action::NONE;
	return scene_change;
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

	for (auto& it : game_objects)
	{
		it->Render(m_RD);
	}
	for (auto& platform : platforms)
	{
		platform->Render(m_RD);
	}
	m_RD->m_spriteBatch->End();
}

void ArenaSelectScene::ReadInput(Input * input_manager)
{
	for (int i = 0; i < no_players; i++)
	{
		if (input_manager->inputs[i] == Inputs::LEFT)
		{
			if (level_selected > 0)
			{
				new_level = true;
				level_selected--;
			}
		}
		if (input_manager->inputs[i] == Inputs::RIGHT)
		{
			if (level_selected < total_levels - 1)
			{
				new_level = true;
				level_selected++;
			}
		}

		if (input_manager->inputs[i] == Inputs::A)
		{
			game_state_data->arena_selected = level_selected;
			action = Action::CONTINUE;
		}

		if (input_manager->inputs[i] == Inputs::START)
		{
			action = Action::BACK;
		}
	}
}

void ArenaSelectScene::loadLevel(RenderData* m_RD, string lvlname)
{
	platforms.clear();

	level = std::make_unique<LevelFile>();
	level->read(lvlname, ".lvl");

	//level_name_text->SetText(lvlname);

	for (int i = 0; i < level->getObjListSize(); i++)
	{
		string temp_name = level->getObj(i).image_file;
		auto platform = new ImageGO2D(m_RD, temp_name, image_buffer);

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