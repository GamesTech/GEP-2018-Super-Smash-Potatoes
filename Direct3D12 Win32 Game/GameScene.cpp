#include "pch.h"
#include "GameScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"


GameScene::~GameScene()
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

	platforms.shrink_to_fit();
	objects.shrink_to_fit();
}

void GameScene::init(RenderData* m_RD, GameStateData* gsd)
{
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

	objects.emplace_back(new Text2D("Super Trash Potatoes"));

	for (auto& object : objects)
	{
		object->SetLayer(1.0f);
	}

	no_players = gsd->no_players;
	if (no_players == 0)
	{
		//for playtesting
		no_players = 1;

	}

	spawnPlayers(gsd, m_RD, no_players);

	//UI
	timer_text = new Text2D("Time Remaining: xxxs");
	timer_text->SetPos(Vector2(750, 10));
	timer_text->SetLayer(1.0f);
	objects.emplace_back(timer_text);

	player_UI_Boxes = new ImageGO2D(m_RD, "PlayerPreviewBoxes");
	player_UI_Boxes->SetPos(Vector2(640, 640));
	player_UI_Boxes->SetRect(1, 1, 723, 180);
	player_UI_Boxes->SetLayer(0.1f);
	player_UI_Boxes->CentreOrigin();
	player_UI_Boxes->SetScale(Vector2(0.75f, 0.75f));
	player_UI_Boxes->SetColour(DirectX::SimpleMath::Color::Color(1, 1, 1, 0.5f));
	objects.emplace_back(player_UI_Boxes);


	/*add lives, damage taken and kills to boxes*/
	for (int i = 0; i < no_players; i++)
	{
		damage_text[i] = new Text2D("xxx%");
		damage_text[i]->SetPos(Vector2(385 + (i * 135), 655));
		damage_text[i]->SetLayer(1.0f);
		damage_text[i]->CentreOrigin();
		damage_text[i]->SetColour(DirectX::SimpleMath::Color::Color(0, 0, 0, 1));
		objects.emplace_back(damage_text[i]);
	}
}

void GameScene::update(GameStateData* gsd)
{
	time_remaining = time_remaining - gsd->m_dt;
	timer_text->SetText("Time Remaining: " + std::to_string(time_remaining) + "s");

	int players_dead = 0;
	for (int i = 0; i < no_players; i++)
	{
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
						if (OtherCollision(platform.get(), i) && !m_anim_grounded[i])
						{
							m_anim_grounded[i] = true;
							break;
						}
					}
				}
			}
			m_player[i]->SetAnimGrounded(m_anim_grounded[i]);
			m_player[i]->Tick(gsd, i);

			m_anim_grounded[i] = false;
			int player_damage = (m_player[i]->GetDamage() * 100) - 100;

			damage_text[i]->SetText(std::to_string(player_damage) + "%");

			for (int j = 0; j < max_lives - (m_player[i]->GetLivesRemaining()); j++)
			{
				lives_button_sprite[(i * 3) + j]->SetColour(DirectX::SimpleMath::Color(1, 0, 0));
			}


			if (m_player[i]->Attack())
			{
				CheckAttackPos(gsd, i);
			}
		}
		else
		{
			players_dead++;
		}
	}
	if (time_remaining <= 0 || (no_players - 1) <= players_dead)
	{
		gsd->gameState = GAMEOVER;
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

	m_RD->m_spriteBatch->End();
}

void GameScene::ReadInput(GameStateData* gsd)
{
	if ((gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape) 
		|| (gsd->m_gamePadState[0].IsStartPressed() && !gsd->m_prevGamePadState[0].IsStartPressed()))
	{
		gsd->gameState = MENU;
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
		std::string str_player_no = sprite_names[gsd->player_selected[i]] + "_batch_" + std::to_string(i);
		m_player[i] = std::make_unique<Player2D>(m_RD, str_player_no);
		m_player[i]->SetPos(Vector2(250, 200));
		m_player[i]->SetLayer(0.5f);
		m_player[i]->SetDrive(900.0f);
		m_player[i]->SetDrag(3.f);
		m_player[i]->LoadSprites(sprite_names[gsd->player_selected[i]] + "_batch.txt");
		m_player[i]->setPlayerNo(i);

		ImageGO2D* temp_player_UI = new ImageGO2D(m_RD, sprite_names[gsd->player_selected[i]]);
		temp_player_UI->SetPos(Vector2(415 + (i * 135), 630));
		temp_player_UI->SetRect(1, 1, 60, 75);
		temp_player_UI->SetLayer(0.0f);
		temp_player_UI->CentreOrigin();
		objects.emplace_back(temp_player_UI);

		max_lives = m_player[0]->GetLivesRemaining();
		for (int j = 0; j < max_lives; j++)
		{
			int lives_no = (i * max_lives) + j;
			lives_button_sprite[lives_no] = new ImageGO2D(m_RD, "lives");
			lives_button_sprite[lives_no]->SetPos(Vector2(480 + (i * 135), 595 + (j * 30)));
			lives_button_sprite[lives_no]->SetRect(1, 1, 20, 20);
			lives_button_sprite[lives_no]->SetLayer(0.0f);
			lives_button_sprite[lives_no]->CentreOrigin();
			objects.emplace_back(lives_button_sprite[lives_no]);
		}
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

void GameScene::CheckAttackPos(GameStateData * _GSD, int _i)
{
	float r1 = m_player[_i]->Width()/1.5;
	float x1 = m_player[_i]->GetPos().x + (m_player[_i]->Width() / 2);
	float y1 = m_player[_i]->GetPos().y + (m_player[_i]->Height() / 2);
	float punch_direction = 0;
	if (m_player[_i]->UpPuch())
	{
		y1 -= 30;
	}
	else if (m_player[_i]->GetOrientation())
	{
		x1 += 40;
		punch_direction = 1;
	}
	else
	{
		x1 -= 40;
		punch_direction = -1;
	}

	for (int j = 0; j < no_players; j++)
	{
		if (_i != j)
		{
			float r2 = m_player[j]->Width();
			//float distance_1 = collision_width - player_width;
			//float distance_2 = collision_width + player_width;
			float x2 = m_player[j]->GetPos().x + (m_player[j]->Width() / 2);
			float y2 = m_player[j]->GetPos().y + (m_player[j]->Height() / 2);

			if (r1 > sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1))))
			{
				if (m_player[_i]->UpPuch())
				{
					m_player[j]->UpHit(_GSD);
				}
				else
				{
					m_player[j]->Hit(_GSD, punch_direction);
				}
			}
		}
	}
	m_player[_i]->Attack(false);
}

