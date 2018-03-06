#include "pch.h"
#include "GameScene.h"
#include "RenderData.h"
#include "GameObject2D.h"
#include "GameStateData.h"


GameScene::~GameScene()
{
	if (m_player)
	{
		delete m_player;
		m_player = nullptr;
	}
	
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
}

void GameScene::init(RenderData* m_RD)
{
	Text2D* title_text = new Text2D("Super Trash Potatoes");
	title_text->SetLayer(1.0f);
	objects.push_back(title_text);

	//game_objects.push_back(m_player);

	
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

	
	//declare platforms before player
	m_player = new Player2D(m_RD, "mario_sprite_batch");
	m_player->loadSprites("MarioSpriteBatch.txt");
	m_player->SetPos(Vector2(300, 300));
	m_player->SetLayer(1.0f);
	m_player->SetDrive(1000.0f);
	m_player->SetDrag(5.f);
	m_player->SetRect(724, 0, 775, 63);
	m_player->SetSpeedLimit(1);
}

void GameScene::update(GameStateData* gsd)
{
	for (std::vector<GameObject2D *>::iterator it = platforms.begin(); it != platforms.end(); it++)
	{
		if (CheckCollision(*it))
		{
			break;
		}
	}
	m_player->Tick(gsd);
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

	m_player->Render(m_RD);
	m_RD->m_spriteBatch->End();
}

void GameScene::ReadInput(GameStateData* gsd)
{
	if (gsd->m_keyboardState.Escape && !gsd->m_prevKeyboardState.Escape)
	{
		gsd->gameState = MENU;
	}
}

bool GameScene::CheckCollision(GameObject2D *_obj)
{
	GameObject2D* object = _obj;

	float width = 0.5 * (m_player->Width() + object->Width());
	float height = 0.5 * (m_player->Height() + object->Height());
	float distance_x = m_player->CenterX() - object->CenterX();
	float distance_y = m_player->CenterY() - object->CenterY();

	if (abs(distance_x) <= width && abs(distance_y) <= height)
	{
		// collision occured

		float collision_width = width * distance_y;
		float collision_height = height * distance_x;

		if (collision_width > collision_height)
		{
			if (collision_width > -collision_height)
			{
				float newPosY = object->GetPos().y + object->Height();
				m_player->SetPosY(newPosY);
				//m_player->SetVelY(0);
				//m_pos.y = newPosY;
				//m_vel.y = 0.0f;
				//m_grounded = true;

				// collision at the bottom 
			}
			else
			{
				float newPosX = object->GetPos().x - m_player->Width();
				m_player->SetPosX(newPosX);
				//m_player->SetVelX(0);
				m_player->SetGrounded(true);
				//m_pos.x = newPosX;
				//m_vel.x = 0.0f;
				//m_grounded = true;
				// on the left 
			}
		}
		else
		{
			if (collision_width > -collision_height)
			{
				float newPosX = object->GetPos().x + object->Width();
				m_player->SetPosX(newPosX);
				//m_player->SetVelX(0);
				m_player->SetGrounded(true);
				//m_pos.x = newPosX;
				//m_vel.x = 0.0f;
				//m_grounded = true;
				// on the right 
			}
			else
			{
				float newPosY = object->GetPos().y - m_player->Height();
				m_player->SetPosY(newPosY);
				//m_player->SetVelY(0);
				m_player->SetGrounded(true);
				//player->SetPos().y = newPosY;
				//m_vel.y = 0.0f;
				//m_grounded = true;
				// at the top 
			}
		}
		return true;
	}
	else
	{
		m_player->SetGrounded(false);
		return false;
		//m_grounded = false;
	}

}

