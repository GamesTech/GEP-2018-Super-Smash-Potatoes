#include "pch.h"
#include <cmath>
#include "GameScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"


GameScene::~GameScene()
{	
	game_objects.clear();
	m_players.clear();
	platforms.shrink_to_fit();
	objects.shrink_to_fit();
}

bool GameScene::init(RenderData* m_RD, GameStateData* gsd, AudioManager* am, std::shared_ptr<ImageBuffer> ib)
{
	image_buffer = ib;
	
	time_remaining = 180.0f;

	level = std::make_unique<LevelFile>();
	level->read("level" + std::to_string(gsd->arena_selected), ".lvl");

	m_collision_system = std::make_unique<CollisionSystem>();
	m_camera = std::make_unique<GameCamera>();
	m_camera->init(gsd);

	loadCharactersFile("PlayerSprites.txt");

	for (int i = 0; i < level->getObjListSize(); i++)
	{
		string temp_name = level->getObj(i).image_file;
 		auto platform = new ImageGO2D(m_RD, temp_name, image_buffer);

		platform->SetPos(level->getObj(i).position);
		platform->SetOrigin(level->getObj(i).origin);
		platform->SetScale(level->getObj(i).scale);
		platform->SetOri(level->getObj(i).orientation);
		platform->SetLayer(level->getObj(i).layer);
		platform->SetType(level->getObj(i).type);

		platform->SetRect(level->getObj(i).sprite_size_min.x, level->getObj(i).sprite_size_min.y, level->getObj(i).sprite_size_max.x, level->getObj(i).sprite_size_max.y);
		platforms.emplace_back(platform);
	}

	for (auto& object : objects)
	{
		object->SetLayer(1.0f);
	}

	gsd->position_in_podium = gsd->no_players;
	no_players = gsd->no_players;

	particle_system = std::make_shared<ParticleSystem>();
	particle_system->init(m_RD, image_buffer);

	m_player_tag = std::make_unique<PlayerTags>(no_players);
	m_player_tag->Init(m_RD, image_buffer);

	spawnPlayers(gsd, m_RD, no_players);

	//UI
	UI = std::make_unique<UserInterface>();
	UI->init(m_RD, gsd, m_players, sprite_names, image_buffer);

	audio_manager = am;
	//audio_manager->changeLoopTrack(TOBYSOUNDTRACK);
	audio_manager->playSound(QUESTCOMPLETE);

	return true;

}

Scene::SceneChange GameScene::update(GameStateData* gsd)
{
	m_camera->UpdateCamera(m_players);

	UI->update(gsd, m_players, time_remaining);

	
	int players_dead = 0;
	particle_system->update(gsd);

	// movement loop
	for (int i = 0; i < no_players; i++)
	{
		Vector2 temp = m_players[i]->GetPos();
		m_player_tag->SetPlayerPos(i, temp, m_players[i]->GetSize().x);
		if (!m_players[i]->getDead())
		{
			for (auto& platform : platforms)
			{
				if (m_collision_system->ResloveCollision(platform.get(), m_players[i].get())
					&& !m_anim_grounded[i])
				{
					m_anim_grounded[i] = true;
					break;
				}
			}
			m_players[i]->SetAnimGrounded(m_anim_grounded[i]);
			if (input_manager)
			{
				m_players[i]->Tick(gsd, i, input_manager);
			}

			m_anim_grounded[i] = false;
		}
		else
		{
			players_dead++;
		}
	}
	m_player_tag->Update();

	// attack loop
	for (int i = 0; i < no_players; i++)
	{
		if (!m_players[i]->getDead())
		{
			Attacking(i, gsd);
		}
	}
	endGame(players_dead, gsd);

	Scene::SceneChange scene_change;
	switch (action)
	{
	case Action::CONTINUE:
	{
		scene_change.change_type = ChangeType::ADD;
		scene_change.scene = SceneEnum::GAMEOVER;
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

void GameScene::endGame(int players_dead, GameStateData * gsd)
{
	if (time_remaining <= 0 || (no_players) <= players_dead + 1)
	{
		if (!gameEnded)
		{
			particle_system->addParticlesToEmitter(300, Particle_Type::FIREWORK, Vector2(100, 100), 5.f, 0.0f, true, true, { 1,1,1,1 }, 1.f, 200, 200);
			particle_system->addParticlesToEmitter(300, Particle_Type::FIREWORK, Vector2(1280 - 100, 100), 5.f, 0.0f, true, true, { 1,1,1,1 }, 1.f, 200, 200);
			gameEnded = true;
		}
		int player_count = 0;
		for (int i = 0; i < no_players; i++)
		{
			if (gsd->player_podium_position[i] == 0)
			{
				gsd->player_podium_position[i] = 1;
			}
		}
		if (timer >= 5)
		{
			action = Action::CONTINUE;
		}
		else
		{
			timer += gsd->m_dt;
		}
	}
}

void GameScene::Attacking(int i, GameStateData * gsd)
{
	bool block = false;
	switch (m_players[i]->GetAttackType())
	{
	case Attack::FIRST:
		for (int j = 0; j < no_players; j++)
		{
			if (i != j && !m_players[j]->getDead() && !m_players[j]->GetInvincibility())
			{
				if (m_players[i]->CheckBlocking(gsd, m_players[j].get()))
				{
					block = true;
				}
			}
		}
		if (block)
		{
			m_players[i]->Block(gsd);
		}
		else
		{
			for (int j = 0; j < no_players; j++)
			{
				if (i != j && !m_players[j]->getDead() && !m_players[j]->GetInvincibility())
				{
					if (m_players[i]->ExectuePunch(gsd, m_players[j].get()))
					{
						audio_manager->playSound(SLAPSOUND);
					}
				}
			}
		}
		m_players[i]->ResetAttacks();
		break;
	case Attack::SECOND:
		for (int j = 0; j < no_players; j++)
		{
			if (i != j && !m_players[j]->getDead() && !m_players[j]->GetInvincibility())
			{
				if (m_players[i]->ExectueUpPunch(gsd, m_players[j].get()))
				{
					audio_manager->playSound(SLAPSOUND);
				}
			}
		}
		break;
	case Attack::THIRD:
		for (int j = 0; j < no_players; j++)
		{
			if (i != j && !m_players[j]->getDead() && !m_players[j]->GetInvincibility())
			{
				if (m_players[i]->ExectueDownPunch(gsd, m_players[j].get()))
				{
					audio_manager->playSound(SLAPSOUND);
				}
			}
		}
		m_players[i]->ResetAttacks();
		break;
	default:
		break;
	}
}

void GameScene::render(RenderData* m_RD,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	
	m_RD->m_spriteBatch->SetViewport(m_camera->getBGViewport());
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);
	
	for (auto& platform : platforms)
	{
		if (platform->GetLayer() == 1)
		{
			platform->Render(m_RD);
		}
	}
	m_RD->m_spriteBatch->End();

	
	
	m_RD->m_spriteBatch->SetViewport(m_camera->getGameViewport());
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);

	for (auto& object : objects)
	{
		object->Render(m_RD);
	}
	
	for (auto& platform : platforms)
	{
		if (platform->GetLayer() != 1)
		{
			platform->Render(m_RD);
		}
	}

	for (int i = 0; i < no_players; i++)
	{
		m_players[i]->Render(m_RD);
	}
	m_player_tag->Render(m_RD);
	particle_system->render(m_RD);

	m_RD->m_spriteBatch->End();


	//Now UI sprite Batch
	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);
	m_RD->m_spriteBatch->SetViewport(m_camera->getUIViewport());
	if (!gameEnded)
	{
		UI->render(m_RD);
	}
	m_RD->m_spriteBatch->End();
}

void GameScene::ReadInput(Input* im)
{
	input_manager = im;
	if (input_manager->inputs[0] == Inputs::START)
	{
		action = Action::BACK;
	}
}

void GameScene::spawnPlayers(GameStateData* gsd, RenderData* m_RD, int no_players)
{
	for (int i = 0; i < no_players; i++)
	{
		std::string str_player_no = sprite_names[gsd->player_selected[i]] + "_batch_" + "0";
		m_players.emplace_back(new Player2D(m_RD, str_player_no, image_buffer));
		m_players.back()->SetPos(m_spawn_pos[i]);
		m_players.back()->SetLayer(0.5f);
		m_players.back()->SetDrive(900.0f);
		m_players.back()->SetDrag(3.0f);
		m_players.back()->LoadSprites(sprite_names[gsd->player_selected[i]] + "_batch.txt");
		m_players.back()->setPlayerNo(i);
		m_players.back()->SetParticleSystem(particle_system);
	}	
}

void GameScene::loadCharactersFile(string _filename)
{
	std::ifstream character_sprites_loading;
	character_sprites_loading.open(_filename);
	if (character_sprites_loading.is_open())
	{
		while (!character_sprites_loading.eof())
		{
			std::string temp_string;
			character_sprites_loading >> temp_string;
			sprite_names.push_back(temp_string);
		}
	}
	character_sprites_loading.close();
}
