#pragma once
#include "Scene.h"
#include "LevelFile.h"

class ArenaSelectScene : public Scene
{
public:
	ArenaSelectScene();
	~ArenaSelectScene();


	virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am, std::shared_ptr<ImageBuffer> image_buffer) override;
	virtual SceneChange update(GameStateData* gsd) override;
	virtual void render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	virtual void ReadInput(Input* input_manager) override;

private:
	enum Action
	{
		NONE,
		BACK,
		CONTINUE
	};
	Action action = Action::NONE;

	GameStateData* game_state_data;

	std::unique_ptr<ImageGO2D> title_boarder = nullptr;
	std::unique_ptr<ImageGO2D> left_arrow = nullptr;
	std::unique_ptr<ImageGO2D> right_arrow = nullptr;

	std::unique_ptr <LevelFile> level;

	std::vector<std::unique_ptr<GameObject2D>> platforms;

	void loadLevelsFile(string _filename);
	void loadLevel(RenderData* m_RD, string lvlname);
	std::vector<string> level_names;

	bool new_level = false;
	int level_selected = 0;
	int total_levels = 0;
};