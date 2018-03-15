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

	ImageGO2D* grid_sprite_temp;
	std::vector<ImageGO2D*> grid_sprites;

	ImageGO2D* player_preview_temp;
	std::vector<ImageGO2D*> player_previews;

	int sprites_per_row = 0;

	std::vector<string> sprite_names;

	int selection_player[4] = { 0,0,0,0 };

	int no_players = 0;
};