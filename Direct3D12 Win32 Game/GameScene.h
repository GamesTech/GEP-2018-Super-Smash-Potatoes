//#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:

	GameScene() = default;
	~GameScene() = default;

	void virtual init(RenderData* m_RD) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;

private:
	virtual void ReadInput(GameStateData* gsd) override;

	Player2D* m_player;
	std::vector<GameObject2D*> m_2DPlatforms;


};

