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

	std::string text[4] = { "Player 1","Player 2","Player 3","Player 4" };
	Vector2 podium_pos[4] = { { 570,150 },{ 770,250 },{ 370,350 },{ 970,450 } };

	void loadCharactersFile(string _filename);

	std::vector<string> sprite_names;

	std::unique_ptr<Text2D> winner_text;

	RenderData* m_RD;

};