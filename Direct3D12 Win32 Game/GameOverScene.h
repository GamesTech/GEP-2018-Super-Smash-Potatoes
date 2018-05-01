#pragma once
#include "Scene.h"

class GameOverScene : public Scene
{
public:
	GameOverScene();
	~GameOverScene();

	virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am) override;
	virtual SceneChange update(GameStateData* gsd) override;
	virtual void render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	void virtual ReadInput(GameStateData* gsd) override;

private:
	enum Action
	{
		NONE,
		BACK
	};
	Action action = Action::NONE;

	RECT number_pos[4] = { {0,0,66,60},{ 72,0,136,60 },{ 186,0,252,60 },{ 300,0,366,60 } };

	std::unique_ptr<ImageGO2D> winner_number = nullptr;
	std::unique_ptr<ImageGO2D> winner_text = nullptr;

	RenderData* m_RD;

};