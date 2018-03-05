//#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:

	GameScene() = default;
	virtual ~GameScene();

	void virtual init(RenderData* m_RD, GameStateData* gsd) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;

private:
	virtual void ReadInput(GameStateData* gsd) override;
	void spawnPlayers(RenderData* m_RD, int no_players);

	int no_players = 0;

	Player2D* m_player[4];

	Text2D* title_text;
	ImageGO2D* platform;

};

