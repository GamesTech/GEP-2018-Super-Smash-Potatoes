#pragma once
#include "Scene.h"

class GameOverScene : public Scene
{
public:
	GameOverScene();
	~GameOverScene();

	virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am, std::shared_ptr<ImageBuffer> ib) override;
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

	std::string text[5] = { "Player 1 wins","Player 2 wins","Player 3 wins","Player 4 wins","DRAW" };

	std::unique_ptr<Text2D> winner_text;

	RenderData* m_RD;

};