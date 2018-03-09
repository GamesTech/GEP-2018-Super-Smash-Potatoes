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
	objects.emplace_back(new Text2D("Super Trash Potatoes"));
	for (auto& object : objects)
	{
		object->SetLayer(1.0f);
	}
	for (int i = 0; i < 3; i++)
	{
		platforms.emplace_back(new ImageGO2D(m_RD, "platform"));
	}
	int i = 0;
	for (auto& platform : platforms)
	{
		platform->SetPos(platform_pos[i]);
		platform->SetLayer(1.0f);
		platform->SetRect(1, 1, 600, 72);
		i++;
	}
	
	no_players = gsd->no_players;
	if (no_players == 0)
	{
		no_players = 1;
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
			else
			{
				m_anim_grounded[i] = false;
			}
		}
		m_player[i]->SetAnimGrounded(m_anim_grounded[i]);
		m_player[i]->Tick(gsd);
		m_player[i]->AnimationTick(gsd);
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
		m_player[i]->SetDrag(2.f);
		m_player[i]->LoadSprites("MarioSpriteBatch.txt");
		m_player[i]->SetSpeedLimit(1);
		m_player[i]->setPlayerNo(i);
	}
}

