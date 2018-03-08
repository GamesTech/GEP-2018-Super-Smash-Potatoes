#pragma once
#include "Scene.h"

class CharacterSelectScene : public Scene
{
public:
	CharacterSelectScene();
	~CharacterSelectScene();

	void virtual init(RenderData* m_RD, GameStateData* gsd) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	void virtual ReadInput(GameStateData* gsd) override;

private:
	void loadCharactersFile(string _filename);

	Text2D* title_text;
	ImageGO2D* player_preview_boxes;
	ImageGO2D* selection_option[4];

	ImageGO2D* player1_preview;
	ImageGO2D* player2_preview;
	ImageGO2D* player3_preview;
	ImageGO2D* player4_preview;

	std::vector<string> sprite_names;

	int selection_player[4];
};