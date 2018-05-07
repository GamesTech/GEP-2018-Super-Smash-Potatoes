#pragma once
#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene();
	
	//virtual ~MenuScene();

	virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am, std::shared_ptr<ImageBuffer> ib) override;
	virtual SceneChange update(GameStateData* gsd) override;
	virtual void render(RenderData* m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	virtual void ReadInput(Input* input_manager) override;

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

	std::unique_ptr<ImageGO2D> title_text = nullptr;
	std::unique_ptr<ImageGO2D> button[4] = { nullptr, nullptr, nullptr };
	struct ButtonInfo
	{
		RECT m_rect;
		Vector2 pos;
	};

	ButtonInfo button_info[4] = { {{ 1, 1, 240, 80 },{ 150, 200 }},{ { 1, 321, 240, 400 },{ 150, 300 } },{{ 1, 81, 240, 160 },{ 150, 400 }},{{ 1, 161, 240, 240 },{ 150, 500 }} };

	int menu_option_selected = 1;

	void highlight_option_selected();
};

