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
	bool CheckCollision(GameObject2D * _obj, int _i);

	int no_players = 0;
	Vector2 platform_pos[3] = { Vector2(0, 200), Vector2(200, 400), Vector2(400, 600)};

	std::unique_ptr<Player2D> m_player[4];

	std::vector<std::unique_ptr<GameObject2D>> platforms;
	std::vector<std::unique_ptr<GameObject2D>> objects;
	//Player2D* m_player;
	bool m_anim_grounded[4] = { false,false,false,false };
	//Text2D* title_text;
	//ImageGO2D* platform;

};

