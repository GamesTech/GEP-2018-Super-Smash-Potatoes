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
	virtual void ReadInput(GameStateData* gsd) override;

private:
	enum Action
	{
		NONE,
		CONTINUE,
		BACK
	};

	Action action = Action::NONE;

	void loadCharactersFile(string _filename);

	std::unique_ptr<Text2D> title_text = nullptr;
	std::unique_ptr<ImageGO2D> player_preview_boxes = nullptr;

	std::unique_ptr<ImageGO2D> grid_sprite_temp = nullptr;
	std::vector<std::unique_ptr<ImageGO2D>> grid_sprites;

	std::unique_ptr<ImageGO2D> player_preview_temp = nullptr;
	std::vector<std::unique_ptr<ImageGO2D>> player_previews;

	int sprites_per_row = 0;

	std::vector<string> sprite_names;

	int selection_player[4] = { 0,0,0,0 };
	bool players_locked[4] = { false, false, false, false };

	int no_players = 0;

};