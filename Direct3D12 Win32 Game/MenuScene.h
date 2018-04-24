#pragma once
#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene();
	
	//virtual ~MenuScene();

	virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am) override;
	virtual SceneChange update(GameStateData* gsd) override;
	virtual void render(RenderData* m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	virtual void ReadInput(GameStateData* gsd) override;

private:
	enum Action
	{
		NONE,
		START,
		SETTINGS,
		EXIT,
		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_PRESSED
	};

	Action action = Action::NONE;

	std::unique_ptr<Text2D> title_text = nullptr;
	std::unique_ptr<ImageGO2D> start_game_button = nullptr;
	std::unique_ptr<ImageGO2D> settings_button = nullptr;
	std::unique_ptr<ImageGO2D> quit_button = nullptr;

	int menu_option_selected = 1;

	void highlight_option_selected();
};

