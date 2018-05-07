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
	void virtual ReadInput(Input* input_manager) override;

private:
	enum Action
	{
		NONE,
		BACK
	};
	Action action = Action::NONE;

	std::string text[4] = { "1st Place","2nd Place","3rd Place","4th Place" };


	std::unique_ptr<Text2D> winner_text;

	RenderData* m_RD;

};