#pragma once
#include "Scene.h"
#include "LevelFile.h"

class LevelEditor : public Scene
{
public:
	LevelEditor() = default;
	~LevelEditor();

	virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am, std::shared_ptr<ImageBuffer> ib) override;
	virtual SceneChange update(GameStateData* gsd) override;
	virtual void render(RenderData* m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	virtual void ReadInput(Input* input_manager) override;

private:
	enum Action
	{
		NONE,
		START = 8,
		ADD_BLOCK = 4,
		BUTTON_UP = 0,
		BUTTON_DOWN = 1,
		BUTTON_LEFT = 2,
		BUTTON_RIGHT = 3,
		RB = 10,
		LB = 9,
		PAUSE = 8,
		B = 6,
		Y = 7

	};
	Action action = Action::NONE;
	RenderData * m_RD = nullptr;

	std::unique_ptr <LevelFile> level;
	std::unique_ptr <LevelFile> background_file;
	std::vector<std::unique_ptr<GameObject2D>> platforms;
	std::vector<std::unique_ptr<GameObject2D>> backgrounds;

	void createNewObject(int type);
	void loadBackgrounds();
	void moveLastObject(float x, float y);
	void saveLevel();


private:
	
	std::unique_ptr<ImageGO2D> ui = nullptr;
	std::unique_ptr<Text2D> ui_text = nullptr;
	std::unique_ptr<ImageGO2D> deathzone = nullptr;

	int object_type = 1;
	int background = 0;
	D3D12_VIEWPORT viewport;
	D3D12_VIEWPORT UI_viewport;
};