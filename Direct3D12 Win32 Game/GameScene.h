//#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:

	GameScene() = default;
	~GameScene();

	void virtual init(RenderData* m_RD) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;

private:
	virtual void ReadInput(GameStateData* gsd) override;
	void CheckCollision(GameObject2D * _player, GameObject2D * _obj);
	std::vector<GameObject2D*> platforms;
	std::vector<GameObject2D*> objects;
	Player2D* m_player;
	//Text2D* title_text;
	//ImageGO2D* platform;

};

