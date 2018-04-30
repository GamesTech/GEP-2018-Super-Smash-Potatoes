#include "pch.h"
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

bool GameScene::init(RenderData* m_RD, GameStateData* gsd, AudioManager* am)
{
	m_RD->m_resourceCount = 30;
	time_remaining = 180.0f;

	level = std::make_unique<LevelFile>();
	level->read("level" + std::to_string(gsd->arena_selected), ".lvl");

	loadCharactersFile("PlayerSprites.txt");

	for (int i = 0; i < level->getObjListSize(); i++)
	{
		string temp_name = level->getObj(i).image_file;
 		auto platform = new ImageGO2D(m_RD, temp_name);

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
	//if (no_players == 1)
	//{
	//	//for playtesting
	//	no_players = 2;
	//}

	particle_system = std::make_shared<ParticleSystem>();
	particle_system->init(m_RD);

	m_player_tag = std::make_unique<PlayerTags>(no_players);
	m_player_tag->Init(m_RD);

	spawnPlayers(gsd, m_RD, no_players);

	//UI
	UI = std::make_unique<UserInterface>();
	UI->init(m_RD, gsd, m_player, sprite_names);

	audio_manager = am;
	//audio_manager->changeLoopTrack(TOBYSOUNDTRACK);
	audio_manager->playSound(QUESTCOMPLETE);

	return true;

}

Scene::SceneChange GameScene::update(GameStateData* gsd)
{
	UI->update(gsd, m_player, time_remaining);
	
	int players_dead = 0;
	particle_system->update(gsd);

	// movement loop
	for (int i = 0; i < no_players; i++)
	{
		Vector2 temp = m_player[i]->GetPos();
		m_player_tag->SetPlayerPos(i, temp, m_player[i]->GetSize().x);
		if (m_player[i]->getDead() == false)
		{
			for (auto& platform : platforms)
			{
				if (platform->GetLayer() == 0.5)
				{
					if (platform->GetType() == 0)
					{
						if (MainCollision(platform.get(), i) && !m_anim_grounded[i])
						{
							m_anim_grounded[i] = true;
							break;
						}
					}
					else
					{
						if (!m_player[i]->IgnoreCollision())
						{
							if (OtherCollision(platform.get(), i) && !m_anim_grounded[i])
							{
								m_anim_grounded[i] = true;
								break;
							}
						}
					}
				}
			}
			m_player[i]->SetAnimGrounded(m_anim_grounded[i]);
			m_player[i]->Tick(gsd, i);

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
		if (!m_player[i]->getDead())
		{
			Attacking(i, gsd);
		}
	}

	if (time_remaining <= 0 || (no_players) <= players_dead + 1)
	{
		//action = Action::CONTINUE;
		for (int i = 0; i < no_players; i++)
		{
			if (m_player[i]->getDead() == false)
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
	if (m_player[i]->IsPunching())
	{
		bool block = false;
		for (int j = 0; j < no_players; j++)
		{
			if (i != j && !m_player[j]->getDead() && !m_player[j]->GetInvincibility())
			{
				if (m_player[i]->CheckBlocking(gsd, m_player[j].get()))
				{
					block = true;
				}
			}
		}
		if (block)
		{
			m_player[i]->Block(gsd);
		}
		else
		{
			for (int j = 0; j < no_players; j++)
			{
				if (i != j && !m_player[j]->getDead() && !m_player[j]->GetInvincibility())
				{
					if (m_player[i]->ExectuePunch(gsd, m_player[j].get()))
					{
						audio_manager->playSound(SLAPSOUND);
					}
				}
			}
		}
		m_player[i]->ResetAttacks(false);
	}
	else if (m_player[i]->IsUpPuching())
	{
		for (int j = 0; j < no_players; j++)
		{
			if (i != j && !m_player[j]->getDead() && !m_player[j]->GetInvincibility())
			{
				if (m_player[i]->ExectueUpPunch(gsd, m_player[j].get()))
				{
					audio_manager->playSound(SLAPSOUND);
				}
			}
		}
	}
}

void GameScene::render(RenderData* m_RD,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
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
		m_player[i]->Render(m_RD);
	}
	m_player_tag->Render(m_RD);
	particle_system->render(m_RD);

	UI->render(m_RD);

	m_RD->m_spriteBatch->End();
}

void GameScene::ReadInput(GameStateData* gsd)
{
	if ((gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape) 
		|| (gsd->m_gamePadState[0].IsStartPressed() && !gsd->m_prevGamePadState[0].IsStartPressed()))
	{
		action = Action::BACK;
	}
}

bool GameScene::MainCollision(GameObject2D *_obj, int _i)
{
	GameObject2D* object = _obj;

	float width = 0.5 * (m_player[_i]->Width() + object->Width());
	float height = 0.5 * (m_player[_i]->Height() + object->Height());
	float distance_x = m_player[_i]->CenterX() - object->CenterX();
	float distance_y = m_player[_i]->CenterY() - object->CenterY();

	if (abs(distance_x) <= width && abs(distance_y) <= height)
	{
		// collision occured

		float collision_width = width * distance_y;
		float collision_height = height * distance_x;

		if (collision_width < collision_height)
		{
			if (collision_width < -collision_height)
			{
				if (m_player[_i]->GetCurrVel().y >= 0)
				{
					m_player[_i]->SetNewPos(object->GetPos().y - m_player[_i]->Height());
					m_player[_i]->SetCollState(m_player[_i]->COLTOP);
					return true;
					// at the top 
				}
				else
				{
					m_player[_i]->SetCollState(m_player[_i]->COLNONE);
					return false;
				}
			}
			else if (collision_width > -collision_height)
			{
				if (!m_player[_i]->GetLedgeJump())
				{
					m_player[_i]->SetNewPos(object->GetPos().x + object->Width());
					m_player[_i]->SetCollState(m_player[_i]->COLRIGHT);
					return true;
					// on the right 
				}
				else
				{
					m_player[_i]->SetCollState(m_player[_i]->COLNONE);
					return false;
				}
			}
		}
		else if(collision_width > collision_height)
		{
			if (collision_width > -collision_height)
			{
				if (m_player[_i]->GetCurrVel().y < 0)
				{
					m_player[_i]->SetNewPos(object->GetPos().y + object->Height());
					m_player[_i]->SetCollState(m_player[_i]->COLBOTTOM);
					return true;
					// collision at the bottom 
				}
				else
				{
					m_player[_i]->SetCollState(m_player[_i]->COLNONE);
					return false;
				}
			}
			else if (collision_width < -collision_height)
			{
				if (!m_player[_i]->GetLedgeJump())
				{
					m_player[_i]->SetNewPos(object->GetPos().x - m_player[_i]->Width());
					m_player[_i]->SetCollState(m_player[_i]->COLLEFT);
					return true;
					// on the left 
				}
				else
				{
					m_player[_i]->SetCollState(m_player[_i]->COLNONE);
					return false;
				}
			}
		}
	}
	else
	{
		m_player[_i]->SetCollState(m_player[_i]->COLNONE);
		return false;
	}
}


bool GameScene::OtherCollision(GameObject2D *_obj, int _i)
{
	GameObject2D* object = _obj;

	float width = 0.5 * (m_player[_i]->Width() + object->Width());
	float height = 0.5 * (m_player[_i]->Height() + object->Height());
	float distance_x = m_player[_i]->CenterX() - object->CenterX();
	float distance_y = m_player[_i]->CenterY() - object->CenterY();

	if (abs(distance_x) <= width && abs(distance_y) <= height)
	{
		// collision occured

		float collision_width = width * distance_y;
		float collision_height = height * distance_x;

		if (collision_width < collision_height)
		{
			if (collision_width < -collision_height)
			{
				if (m_player[_i]->GetCurrVel().y >= 0)
				{
					m_player[_i]->SetNewPos(object->GetPos().y - m_player[_i]->Height());
					m_player[_i]->SetCollState(m_player[_i]->COLTOP);
					return true;
					// at the top 
				}
				else
				{
					m_player[_i]->SetCollState(m_player[_i]->COLNONE);
					return false;
				}
			}
			else
			{
				m_player[_i]->SetCollState(m_player[_i]->COLNONE);
				return false;
			}
		}
		else
		{
			m_player[_i]->SetCollState(m_player[_i]->COLNONE);
			return false;
		}
	}
	else
	{
		m_player[_i]->SetCollState(m_player[_i]->COLNONE);
		return false;
	}
}

void GameScene::spawnPlayers(GameStateData* gsd, RenderData* m_RD, int no_players)
{
	for (int i = 0; i < no_players; i++)
	{
		std::string str_player_no = sprite_names[gsd->player_selected[i]] + "_batch_" + "0";
		m_player[i] = std::make_unique<Player2D>(m_RD, str_player_no);
		//m_player[i]->init(audio_manager);
		m_player[i]->SetPos(m_spawn_pos[i]);
		m_player[i]->SetLayer(0.5f);
		m_player[i]->SetDrive(900.0f);
		m_player[i]->SetDrag(3.0f);
		m_player[i]->LoadSprites(sprite_names[gsd->player_selected[i]] + "_batch.txt");
		m_player[i]->setPlayerNo(i);
		m_player[i]->SetParticleSystem(particle_system);
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

//void GameScene::CheckAttackPos(GameStateData * _GSD, int _i)
//{
//	float r1 = m_player[_i]->Width()/1.5;
//	float x1 = m_player[_i]->GetPos().x + (m_player[_i]->Width() / 2);
//	float y1 = m_player[_i]->GetPos().y + (m_player[_i]->Height() / 2);
//	float punch_direction = 0;
//	float block = false;
//	
//	if (m_player[_i]->GetOrientation())
//	{
//		x1 += 40;
//		punch_direction = 1;
//	}
//	else
//	{
//		x1 -= 40;
//		punch_direction = -1;
//	}
//
//	//block
//	for (int j = 0; j < no_players; j++)
//	{
//		if (_i != j)
//		{
//			if (m_player[j]->IsPunching() && punch_direction != m_player[j]->GetOrientation())
//			{
//				float r2 = m_player[j]->Width();
//				//float distance_1 = collision_width - player_width;
//				//float distance_2 = collision_width + player_width;
//				float x2 = m_player[j]->GetPos().x + (m_player[j]->Width() / 2);
//				float y2 = m_player[j]->GetPos().y + (m_player[j]->Height() / 2);
//
//				if (r1 > sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1))))
//				{
//					m_player[j]->Block(_GSD);
//					m_player[j]->ResetAttacks(false);
//					block = true;
//				}
//			}
//		}
//	}
//	if (block)
//	{
//		m_player[_i]->Block(_GSD);
//	}
//	else
//	{
//		// standard punch
//		for (int j = 0; j < no_players; j++)
//		{
//			if (_i != j && !m_player[j]->GetInvincibility())
//			{
//				float r2 = m_player[j]->Width();
//				float x2 = m_player[j]->GetPos().x + (m_player[j]->Width() / 2);
//				float y2 = m_player[j]->GetPos().y + (m_player[j]->Height() / 2);
//
//				if (r1 > sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1))))
//				{
//					m_player[j]->GotHit(_GSD, punch_direction);
//					audio_manager->playSound(SLAPSOUND);
//				}
//			}
//		}
//	}
//	m_player[_i]->ResetAttacks(false);
//}
//
//void GameScene::CheckUpAttackPos(GameStateData * _GSD, int _i)
//{
//	float r1 = m_player[_i]->Width() / 1.5;
//	float x1 = m_player[_i]->GetPos().x + (m_player[_i]->Width() / 2);
//	float y1 = m_player[_i]->GetPos().y + (m_player[_i]->Height() / 2);
//	float punch_direction = 0;
//	float block = false;
//	if (m_player[_i]->IsUpPuching())
//	{
//		y1 -= 30;
//	}
//	// standard punch
//	for (int j = 0; j < no_players; j++)
//	{
//		if (_i != j)
//		{
//			float r2 = m_player[j]->Width();
//			//float distance_1 = collision_width - player_width;
//			//float distance_2 = collision_width + player_width;
//			float x2 = m_player[j]->GetPos().x + (m_player[j]->Width() / 2);
//			float y2 = m_player[j]->GetPos().y + (m_player[j]->Height() / 2);
//
//			if (r1 > sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1))))
//			{
//				if (!m_player[j]->GetImmune())
//				{
//					m_player[j]->GotUpHit(_GSD);
//					audio_manager->playSound(SLAPSOUND);
//				}
//			}
//		}
//	}
//
//	//m_player[_i]->Attack(false);
//}
