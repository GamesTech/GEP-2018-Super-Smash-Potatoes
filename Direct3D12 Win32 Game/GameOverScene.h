#pragma once
#include "Scene.h"

class GameOverScene : public Scene
{
public:
	GameOverScene();
	~GameOverScene();

	void virtual init(RenderData* m_RD, GameStateData* gsd, AudioManager* am) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	void virtual ReadInput(GameStateData* gsd) override;

private:
	Text2D* game_over_text;
	Text2D* winner_text;
};