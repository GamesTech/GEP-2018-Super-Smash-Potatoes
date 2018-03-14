#include "pch.h"
#include "GameScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"


GameScene::~GameScene()
{	
	platforms.shrink_to_fit();
	objects.shrink_to_fit();
}

void GameScene::init(RenderData* m_RD, GameStateData* gsd)
{
	level = std::make_unique<LevelFile>();
	level->read("test2", ".huwsucks");

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

	objects.emplace_back(new Text2D("Super Trash Potatoes"));
	for (auto& object : objects)
	{
		object->SetLayer(1.0f);
	}

	no_players = gsd->no_players;
	if (no_players == 0)
	{
		no_players = 2;
	}
	spawnPlayers(m_RD, no_players);
}

void GameScene::update(GameStateData* gsd)
{
	for (int i = 0; i < no_players; i++)
	{
		for (auto& platform : platforms)
		{
			if (CheckCollision(platform.get(), i) && !m_anim_grounded[i])
			{
				m_anim_grounded[i] = true;
				break;
			}
		}
		m_player[i]->SetAnimGrounded(m_anim_grounded[i]);
		m_player[i]->Tick(gsd, i);

		if (m_player[i]->Attack())
		{
			CheckAttackPos(gsd, i);
		}
	}
	for (int i = 0; i < no_players; i++)
	{
		m_anim_grounded[i] = false;
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

bool GameScene::CheckCollision(GameObject2D *_obj, int _i)
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
					// at the top 
				}
				else
				{
					m_player[_i]->SetCollState(m_player[_i]->COLNONE);
					return false;
				}
			}
			else if (collision_width > -collision_height) //if (m_player[_i]->GetCurrVel().x <= 0)
			{
				if (!m_player[_i]->GetLedgeJump())
				{
					m_player[_i]->SetNewPos(object->GetPos().x + object->Width());
					m_player[_i]->SetCollState(m_player[_i]->COLRIGHT);
				}
				else
				{
					m_player[_i]->SetCollState(m_player[_i]->COLNONE);
					return false;
				}

				// on the right 
			}
			//else
			//{
			//	m_player[_i]->SetCollState(m_player[_i]->COLNONE);
			//	return false;
			//}
			
		}
		else if(collision_width > collision_height)
		{
			if (collision_width > -collision_height)
			{
				if (m_player[_i]->GetCurrVel().y < 0)
				{
					m_player[_i]->SetNewPos(object->GetPos().y + object->Height());
					m_player[_i]->SetCollState(m_player[_i]->COLBOTTOM);
					// collision at the bottom 
				}
				else
				{
					m_player[_i]->SetCollState(m_player[_i]->COLNONE);
					return false;
				}
			}
			else if (collision_width < -collision_height)//if (m_player[_i]->GetCurrVel().x >= 0)
			{
				if (!m_player[_i]->GetLedgeJump())
				{
					m_player[_i]->SetNewPos(object->GetPos().x - m_player[_i]->Width());
					m_player[_i]->SetCollState(m_player[_i]->COLLEFT);
				}
				else
				{
					m_player[_i]->SetCollState(m_player[_i]->COLNONE);
					return false;
				}
				// on the left 
			}
			//else
			//{
			//	m_player[_i]->SetCollState(m_player[_i]->COLNONE);
			//	return false;
			//}
		}
		return true;
	}
	else
	{
		m_player[_i]->SetCollState(m_player[_i]->COLNONE);
		return false;
	}
}

void GameScene::spawnPlayers(RenderData* m_RD, int no_players)
{
	for (int i = 0; i < no_players; i++)
	{
		std::string str_player_no = "kirby_sprite_batch_" + std::to_string(i);
		m_player[i] = std::make_unique<Player2D>(m_RD, str_player_no);
		m_player[i]->SetPos(Vector2(250, 200));
		m_player[i]->SetLayer(1.0f);
		m_player[i]->SetDrive(900.0f);
		m_player[i]->SetDrag(3.f);
		m_player[i]->LoadSprites("KirbySpriteBatch.txt");
		m_player[i]->setPlayerNo(i);
	}
}

void GameScene::CheckAttackPos(GameStateData * _GSD, int _i)
{
	float collision_width = m_player[_i]->Width()/3;
	float coll_pos_x = m_player[_i]->GetPos().x + (m_player[_i]->Width() / 2);
	float coll_pos_y = m_player[_i]->GetPos().y + (m_player[_i]->Height() / 2);
	float punch_direction = 0;
	if (m_player[_i]->GetOrientation())
	{
		coll_pos_x -= 50;
		punch_direction = 1;
	}
	else
	{
		coll_pos_x += 50;
		punch_direction = -1;
	}

	for (int j = 0; j < no_players; j++)
	{
		if (_i != j)
		{
			float player_width = m_player[j]->Width();
			float distance_1 = collision_width - player_width;
			float distance_2 = collision_width + player_width;
			float distance_x = coll_pos_x - (m_player[j]->GetPos().x + (m_player[j]->Width() / 2));
			float distance_y = coll_pos_y - (m_player[j]->GetPos().y + (m_player[j]->Height() / 2));

			if (distance_1*distance_1 <= (distance_x*distance_x) + (distance_y*distance_y)
				&& (distance_x * distance_x) + (distance_y * distance_y) <= distance_2 * distance_2)
			{
				m_player[j]->Hit(_GSD, punch_direction);
			}
		}
	}
	m_player[_i]->Attack(false);
}

