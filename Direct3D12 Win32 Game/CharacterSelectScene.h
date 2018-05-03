#pragma once
#include "Scene.h"

class CharacterSelectScene : public Scene
{
public:
	CharacterSelectScene();
	~CharacterSelectScene();

	virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am) override;
	virtual SceneChange update(GameStateData* gsd) override;
	virtual void render(RenderData* m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	virtual void ReadInput(Input* input_manager) override;

private:
	enum Action
	{
		NONE,
		CONTINUE,
		BACK
	};

	Action action = Action::NONE;

	void loadCharactersFile(string _filename);

	std::unique_ptr<ImageGO2D> title_text = nullptr;
	std::unique_ptr<ImageGO2D> player_preview_boxes = nullptr;

	std::unique_ptr<ImageGO2D> grid_sprite_temp = nullptr;
	std::vector<std::unique_ptr<ImageGO2D>> grid_sprites;

	std::unique_ptr<ImageGO2D> player_preview_temp = nullptr;
	std::vector<std::unique_ptr<ImageGO2D>> player_previews;

	std::vector<std::unique_ptr<ImageGO2D>> selection_numbers;

	int sprites_per_row = 0;
	int grid_sprite_pixel_size = 64;
	int sprite_pixel_gap = 150;

	std::vector<string> sprite_names;

	int selection_player[4] = { 0,0,0,0 };
	bool players_locked[4] = { false, false, false, false };

	RECT number_pos[4] = { { 0,0,32,35 },{ 49,0,95,35 },{ 114,0,160,35 },{ 172,0,218,35 } };
	std::unique_ptr<ImageGO2D> player_numbers = nullptr;

	int no_players = 0;

};