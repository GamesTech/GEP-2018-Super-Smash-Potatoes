#pragma once
#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene();
	
	//virtual ~MenuScene();

	void virtual init(RenderData* m_RD, GameStateData* gsd) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	void virtual ReadInput(GameStateData* gsd) override;

private:
	ImageGO2D* start_game_button;
	ImageGO2D* settings_button;
	ImageGO2D* quit_button;
	int menu_option_selected = 1;

	void highlight_option_selected();
};

