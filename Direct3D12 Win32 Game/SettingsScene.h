#pragma once
#include "Scene.h"

class SettingsScene : public Scene
{
public:
	SettingsScene() = default;
	virtual ~SettingsScene();

	void virtual init(RenderData* m_RD) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;

private:
	void virtual  ReadInput(GameStateData* gsd) override;
	void newResolutionText(int new_resolution_option);
	void highlight_option_selected();

	int menu_option_selected = 1;
	int resolution_option_selected = 1;

	Text2D* resolution_text;
	ImageGO2D* main_menu_button;

	int m_outputWidth = 0;
	int m_outputHeight = 0;
};
