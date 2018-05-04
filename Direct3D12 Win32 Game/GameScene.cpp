#include "pch.h"
#include <cmath>
#include "GameScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"


GameScene::~GameScene()
{	
	game_objects.clear();
	platforms.shrink_to_fit();
	objects.shrink_to_fit();
}

bool GameScene::init(RenderData* m_RD, GameStateData* gsd, AudioManager* am, std::shared_ptr<ImageBuffer> ib)
{
	image_buffer = ib;
	
	time_remaining = 180.0f;

	x_resolution = gsd->x_resolution;
	y_resolution = gsd->y_resolution;

	level = std::make_unique<LevelFile>();
	level->read("level" + std::to_string(gsd->arena_selected), ".lvl");

	m_collision_system = std::make_unique<CollisionSystem>();

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
	gsd->camera_view_width = 1280;
	gsd->camera_view_height = 720;
	x_zoom_resolution = 1280;
	y_zoom_resolution = 720;

	return true;

}

Scene::SceneChange GameScene::update(GameStateData* gsd)
{
	calculateCameraPosition();

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
			m_players[i]->Tick(gsd, i);

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

	if (time_remaining <= 0 || (no_players) <= players_dead + 1)
	{
		if (!gameEnded)
		{
			particle_system->addParticlesToEmitter(300, Particle_Type::FIREWORK, Vector2(100, 100), 5.f, 0.0f, true, true, { 1,1,1,1 }, 1.f, 200, 200);
			particle_system->addParticlesToEmitter(300, Particle_Type::FIREWORK, Vector2(1280 - 100, 100), 5.f, 0.0f, true, true, { 1,1,1,1 }, 1.f, 200, 200);
			gameEnded = true;
		}
		for (int i = 0; i < no_players; i++)
		{
			if (m_players[i]->getDead() == false)
			{
				switch (i)
				{
				case 0:
					gsd->winState = PLAYER1;
					break;
				case 1:
					gsd->winState = PLAYER2;
					break;
				case 2:
					gsd->winState = PLAYER3;
					break;
				case 3:
					gsd->winState = PLAYER4;
					break;
				}
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

	m_RD->m_spriteBatch->SetViewport(viewport);

	m_RD->m_spriteBatch->Begin(m_commandList.Get(), SpriteSortMode_BackToFront);

	for (auto& object : objects)
	{
		object->Render(m_RD);
	}
	for (auto& platform : platforms)
	{
		platform->Render(m_RD);
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
	UI_viewport = { -1.f, -1.f,
		static_cast<float>(x_resolution), static_cast<float>(y_resolution),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	m_RD->m_spriteBatch->SetViewport(UI_viewport);
	if (!gameEnded)
	{
		UI->render(m_RD);
	}
	m_RD->m_spriteBatch->End();
}

void GameScene::ReadInput(GameStateData* gsd)
{
	if ((gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape) 
		|| (gsd->m_gamePadState[0].IsStartPressed() && !gsd->m_prevGamePadState[0].IsStartPressed()))
	{
		//action = Action::BACK;
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

void GameScene::calculateCameraPosition()
{
	float top_left_x = 0;
	float top_left_y = 0;
	//bool zoom = false;
	Vector2 position;
	int j = 0;
	for (int i = 0; i < no_players; i++)
	{
		if (!m_players[i]->getDead())
		{
			position += m_players[i]->GetPos();
			j++;
		}
	}
	Vector2 centre = Vector2(position.x / j, position.y / j);
	//top_left_x = -(centre.x * (2.f / gsd->camera_view_width) - 1);
	//top_left_y = -(centre.y * (2.f / gsd->camera_view_height) - 1);

	// TEST BROKEN
	{
		Rectangle rect = Rectangle(m_players.back()->GetPos().x, m_players.back()->GetPos().y, 0, 0);
		bool y_zoom_height = false;
		bool x_zoom_width = false;
		bool y_zoom = false;
		bool x_zoom = false;
		int i = 0;
		for (auto& player : m_players)
		{
			if (!player->getDead())
			{
				i++;
				float x = (int)player->GetPos().x;
				float y = (int)player->GetPos().y;

				if (x < rect.x) { rect.x = x; rect.width += rect.x - x; }
				if (x >= (rect.x + rect.width)) { rect.width = x - rect.x; }

				if (y < rect.y) { rect.y = y; rect.height += rect.y - y; }
				if (y >= (rect.y + rect.height)) { rect.height = y - rect.y; }
			}
		}
		{
			rect.x -= 100;
			rect.y -= 100;
			rect.width += 400;
			rect.height += 400;
		}
		Vector2 cameraPos = Vector2((float)rect.x, (float)rect.y);
		cameraPos.x += (float)rect.width / 2;
		cameraPos.y += (float)rect.height / 2;
		float xZoomReq = (float)rect.width / x_zoom_resolution;
		float yZoomReq = (float)rect.height / y_zoom_resolution;
		float zoom = 0;
		if (xZoomReq > yZoomReq) { zoom = xZoomReq; }
		else { zoom = yZoomReq; }
		if (zoom == 0)
		{
			zoom = 1;
		}
		y_zoom_resolution *= zoom;
		x_zoom_resolution *= zoom;

		//Cameara Zoom Limits
		if (x_zoom_resolution < 960) { x_zoom_resolution = 960; }
		if (x_zoom_resolution > 1920) { x_zoom_resolution = 1920; }
		if (y_zoom_resolution > 1080) { y_zoom_resolution = 1080; }
		if (y_zoom_resolution < 540) { y_zoom_resolution = 540; }

		//top_left_x = -(cameraPos.x * (2.f / x_zoom_resolution) - 1);
		//top_left_y = -(cameraPos.y * (2.f / y_zoom_resolution) - 1);
		top_left_x = -(centre.x * (2.f / x_zoom_resolution) - 1);
		top_left_y = -(centre.y * (2.f / y_zoom_resolution) - 1);
	}

	//if (top_left_y > 0.5){top_left_y = 0.5;}
	//if (top_left_y < -0.5){top_left_y = -0.5;}
	//if (top_left_x > 0.5) { top_left_x = 0.5; }
	//if (top_left_x < -0.3) { top_left_x = -0.3; }

	viewport = { -1 + top_left_x, -1 + top_left_y,
		static_cast<float>(x_zoom_resolution), static_cast<float>(y_zoom_resolution),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
}
