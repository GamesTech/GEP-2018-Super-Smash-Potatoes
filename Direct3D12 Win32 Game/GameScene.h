#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:

	GameScene();
	~GameScene();

	void virtual init(RenderData* m_RD) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;

private:

};

