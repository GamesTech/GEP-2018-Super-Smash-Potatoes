#include "pch.h"
#include "GameScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"


GameScene::~GameScene()
{	
	for (auto object : objects)
	{
		if (object)
		{
			delete object;
			object = nullptr;
		}
	}
	for (auto object : platforms)
	{
		if (object)
		{
			delete object;
			object = nullptr;
		}
	}

	platforms.clear();
	objects.clear();
	//delete[] m_player;
}

void GameScene::init(RenderData* m_RD, GameStateData* gsd)
{
	Text2D* title_text = new Text2D("Super Trash Potatoes");
	title_text = new Text2D("Super Trash Potatoes");
	title_text->SetLayer(1.0f);
	objects.push_back(title_text);

	
	ImageGO2D* platform = new ImageGO2D(m_RD, "platform");
	platform->SetPos(Vector2(200, 600));
	platform->SetLayer(1.0f);
	platform->SetRect(1,1,600,72);
	platforms.push_back(platform);

	ImageGO2D* platform2 = new ImageGO2D(m_RD, "platform");
	platform2->SetPos(Vector2(0, 300));
	platform2->SetLayer(1.0f);
	platform2->SetRect(1, 1, 600, 72);
	platforms.push_back(platform2);

	ImageGO2D* platform3 = new ImageGO2D(m_RD, "platform");
	platform3->SetPos(Vector2(800,450));
	platform3->SetLayer(1.0f);
	platform3->SetRect(1, 1, 600, 72);
	platforms.push_back(platform3);

	no_players = gsd->no_players;
	spawnPlayers(m_RD, no_players);

	
	//declare platforms before player
	//m_player = new Player2D(m_RD, "mario_sprite_batch");
	//m_player->loadSprites("MarioSpriteBatch.txt");
	//m_player->SetPos(Vector2(300, 300));
	//m_player->SetLayer(1.0f);
	//m_player->SetDrive(1000.0f);
	//m_player->SetDrag(10.f);
	//m_player->SetRect(724, 0, 775, 63);
	//m_player->SetSpeedLimit(platforms.size());
}

void GameScene::update(GameStateData* gsd)
{
	for (std::vector<GameObject2D *>::iterator it = platforms.begin(); it != platforms.end(); it++)
	{

		for (int i = 0; i < no_players; i++)
		{
			if (CheckCollision(*it, i) && !m_anim_grounded[i])
			{
				m_anim_grounded[i] = true;
			}
			m_player[i]->SetAnimGrounded(m_anim_grounded[i]);
			m_player[i]->Tick(gsd);
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

	for (std::vector<GameObject2D *>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Render(m_RD);
	}
	for (std::vector<GameObject2D *>::iterator it = platforms.begin(); it != platforms.end(); it++)
	{
		(*it)->Render(m_RD);
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

		if (collision_width > collision_height)
		{
			if (collision_width > -collision_height)
			{
				m_player[_i]->SetNewPos(object->GetPos().y + object->Height());
				m_player[_i]->SetCollState(m_player[_i]->COLBOTTOM);
				// collision at the bottom 
			}
			else
			{
				m_player[_i]->SetNewPos(object->GetPos().x - m_player[_i]->Width());
				m_player[_i]->SetCollState(m_player[_i]->COLLEFT);
				// on the left 
			}
		}
		else
		{
			if (collision_width > -collision_height)
			{
				m_player[_i]->SetNewPos(object->GetPos().x + object->Width());
				m_player[_i]->SetCollState(m_player[_i]->COLRIGHT);
				// on the right 
			}
			else
			{
				m_player[_i]->SetNewPos(object->GetPos().y - m_player[_i]->Height());
				m_player[_i]->SetCollState(m_player[_i]->COLTOP);
				// at the top 
			}
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
		std::string str_player_no = "mario_sprite_batch_" + std::to_string(i);
		m_player[i] = std::make_unique<Player2D>(m_RD, str_player_no);
		m_player[i]->SetPos(Vector2(300, 300));
		m_player[i]->SetLayer(1.0f);
		m_player[i]->SetDrive(500.0f);
		m_player[i]->SetDrag(0.5f);
		m_player[i]->LoadSprites("MarioSpriteBatch.txt");
		m_player[i]->SetSpeedLimit(platforms.size());
		m_player[i]->setPlayerNo(i);
	}
}

