#pragma once
#include "Scene.h"

class SettingsScene : public Scene
{
public:
	SettingsScene() = default;
	virtual ~SettingsScene();

	virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am) override;
	virtual SceneChange update(GameStateData* gsd) override;
	virtual void render(RenderData* m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;

	bool new_resolution = false;

private:
	enum Action
	{
		NONE,
		FULLSCREEN,
		SCREEN_RES,
		EXIT
	};

	Action action = Action::NONE;
	void virtual  ReadInput(GameStateData* gsd) override;
	void newResolutionText(int new_resolution_option);
	void highlight_option_selected();
	void virtual giveSwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain);

	int menu_option_selected = 1;
	int resolution_option_selected = 1;
	int new_outputWidth = 0;
	int new_outputHeight = 0;
	bool fullscreen = false;

	Text2D* resolution_text;
	Text2D* fullscreen_text;
	ImageGO2D* main_menu_button;

	Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
};
