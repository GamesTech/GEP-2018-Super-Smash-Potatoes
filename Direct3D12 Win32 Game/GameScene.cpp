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
	level->read("level\\level" + std::to_string(gsd->arena_selected), ".lvl");

	m_collision_system = std::make_unique<CollisionSystem>();
	m_camera = std::make_unique<GameCamera>();
	m_camera->init(gsd);

	loadCharactersFile("player\\PlayerSprites.txt");

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
	audio_manager->changeLoopTrack(TOBYSOUNDTRACK);
	audio_manager->playSound(QUESTCOMPLETE);

	spawner = std::make_unique<ItemSpawner>();
	spawner->init(m_RD, image_buffer, particle_system);
	return true;

}

Scene::SceneChange GameScene::update(GameStateData* gsd)
{
	m_camera->UpdateCamera(m_players);

	UI->update(gsd, m_players, time_remaining);

	
	int players_dead = 0;
	particle_system->update(gsd);

	// movement loop
	for (auto& player : m_players)
	{
		Vector2 temp = player->GetPos();
		m_player_tag->SetPlayerPos(player->getPlayerNo(), temp, player->GetSize().x);
		if (!player->getDead())
		{
			for (auto& platform : platforms)
			{
				if (m_collision_system->ResloveCollision(platform.get(), player.get())
					&& !m_anim_grounded[player->getPlayerNo()])
				{
					m_anim_grounded[player->getPlayerNo()] = true;
					break;
				}
			}
			player->SetAnimGrounded(m_anim_grounded[player->getPlayerNo()]);
			if (input_manager)
			{
				player->Tick(gsd, player->getPlayerNo(), input_manager);
			}
			for (auto& item : spawner->getItems())
			{
				if (m_collision_system->CheckIntersect(item.get(), player.get(), 100.f))
				{
					item->collided(player.get(), gsd);
				}
			}
			m_anim_grounded[player->getPlayerNo()] = false;
		}
		else
		{
			players_dead++;
		}
	}
	m_player_tag->Update();

	// attack loop and items col
	for (auto& player : m_players)
	{
		if (!player->getDead() && player->GetAttackType() != Attack::NONE)
		{
			Attacking(player.get(), gsd);
		}
	}
	endGame(players_dead, gsd);

	if (spawner->getSize() == 0) 
	{
		if (item_spawn_timer >= 3)
		{
			for (int i = 0; i < 1; i++)
			{
				spawner->addItem(Vector2(400 + (i * 1), 300), "bomb", Item::Type::BOMB, 500);
				item_spawn_timer = 0;
			}
		}
		else
		{
			item_spawn_timer += gsd->m_dt;
		}
	}
	else
	{
		for (auto& item : spawner->getItems())
		{
			if (item->getActive())
			{
				for (auto& platform : platforms)
				{
					if (m_collision_system->ResloveCollision(platform.get(), item.get()))
					{
						break;
					}
				}
			}
		}
	}

	spawner->update(gsd);
	Scene::SceneChange scene_change;
	switch (action)
	{
	case Action::CONTINUE:
	{
		scene_change.change_type = ChangeType::ADD;
		scene_change.scene = SceneEnum::GAMEOVER;
		break;
	}
	case Action::PAUSE:
	{
		scene_change.change_type = ChangeType::ADD;
		scene_change.scene = SceneEnum::PAUSE;
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
			particle_system->addParticles(300, Particle_Type::FIREWORK, Vector2(100, 100), true);
			particle_system->addParticles(300, Particle_Type::FIREWORK, Vector2(1180, 100), true);
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

void GameScene::Attacking(Player2D * _player, GameStateData * gsd)
{
	bool block = false;
	switch (_player->GetAttackType())
	{
	case Attack::FIRST:
		for (auto& player : m_players)
		{
			if (_player != player.get() 
				&& !player->getDead() 
				&& !player->GetInvincibility()
				&& m_collision_system->CheckIntersect(_player, player.get(), 0.f, .66f, 40.f, 0, _player->GetDirection()))
			{			
				if (_player->CheckBlocking(gsd, player.get()))
				{
					block = true;
				}
			}
		}
		if (block)
		{
			_player->Block(gsd);
		}
		else
		{
			for (auto& player : m_players)
			{
				if (_player != player.get() 
					&& !player->getDead() 
					&& !player->GetInvincibility()
					&& m_collision_system->CheckIntersect(_player, player.get(), 0.f, .66f, 40.f, 0, _player->GetDirection()))
				{
					if (_player->ExectuePunch(gsd, player.get()))
					{
						audio_manager->playSound(SLAPSOUND);
					}
				}
			}
		}
		_player->ResetAttacks();
		break;
	case Attack::SECOND:
		for (auto& player : m_players)
		{
			if (_player != player.get() 
				&& !player->getDead() 
				&& !player->GetInvincibility()
				&& m_collision_system->CheckIntersect(_player, player.get(), 0.f, .66f, 0.f, -30.f, 0))
			{
				if (_player->ExectueUpPunch(gsd, player.get()))
				{
					audio_manager->playSound(SLAPSOUND);
				}
			}
		}
		break;
	case Attack::THIRD:
		for (auto& player : m_players)
		{
			if (_player != player.get() 
				&& !player->getDead() 
				&& !player->GetInvincibility()
				&& m_collision_system->CheckIntersect(_player, player.get(), 100.f, 1.f, 0.f, _player->Width()/2, 0))
			{
				if (_player->ExectueDownPunch(gsd, player.get()))
				{
					audio_manager->playSound(SLAPSOUND);
				}
			}
		}
		_player->ResetAttacks();
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
	
	spawner->render(m_RD);
	
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
	im->current_scene = CurrentScene::GAME;
	input_manager = im;
	for (int i = 0; i < no_players; i++)
	{
		if (input_manager->inputs[i] == Inputs::ESCAPE)
		{
			action = Action::BACK;
		}
		else if (input_manager->inputs[i] == Inputs::START)
		{
			action = Action::PAUSE;
		}
	}
}

void GameScene::spawnPlayers(GameStateData* gsd, RenderData* m_RD, int no_players)
{
	for (int i = 0; i < no_players; i++)
	{
		std::string str_player_no = sprite_names[gsd->player_selected[i]] + "_batch_" + "0";
		m_players.emplace_back(new Player2D(m_RD, str_player_no, image_buffer, sprite_names[gsd->player_selected[i]]));
		m_players.back()->SetPos(m_spawn_pos[i]);
		m_players.back()->SetLayer(0.5f);
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
