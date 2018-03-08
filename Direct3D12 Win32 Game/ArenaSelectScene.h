#pragma once
#include "Scene.h"

class ArenaSelectScene : public Scene
{
public:
	ArenaSelectScene();
	~ArenaSelectScene();


	void virtual init(RenderData* m_RD, GameStateData* gsd) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	void virtual ReadInput(GameStateData* gsd) override;
private:

};