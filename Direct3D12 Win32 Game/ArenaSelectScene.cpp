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
}

void ArenaSelectScene::init(RenderData * m_RD, GameStateData * gsd)
{
}

void ArenaSelectScene::update(GameStateData * gsd)
{
}

void ArenaSelectScene::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
}

void ArenaSelectScene::ReadInput(GameStateData * gsd)
{
	if ((gsd->m_keyboardState.Enter && !gsd->m_prevKeyboardState.Enter)
		|| (gsd->m_gamePadState[0].IsAPressed() && !gsd->m_prevGamePadState[0].IsAPressed()))
	{
		gsd->gameState = INGAME;
	}
}
