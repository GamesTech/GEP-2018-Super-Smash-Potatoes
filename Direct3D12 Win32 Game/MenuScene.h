#pragma once
#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	//virtual ~MenuScene();

	void virtual init(RenderData* m_RD) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	void virtual ReadInput(GameStateData* gsd) override;

private:
	Text2D* title_text;
	ImageGO2D* start_game_button;
	ImageGO2D* settings_button;
	ImageGO2D* quit_button;
	int menu_option_selected = 1;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;

	void highlight_option_selected();
};

