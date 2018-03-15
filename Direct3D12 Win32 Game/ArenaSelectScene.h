#pragma once
#include "Scene.h"
#include "LevelFile.h"

class ArenaSelectScene : public Scene
{
public:
	ArenaSelectScene();
	~ArenaSelectScene();


	void virtual init(RenderData* m_RD, GameStateData* gsd) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	void virtual ReadInput(GameStateData* gsd) override;

private:
	Text2D* title_text;

	std::unique_ptr <LevelFile> level;

	std::vector<std::unique_ptr<GameObject2D>> platforms;

	void loadLevelsFile(string _filename);
	void loadLevel(RenderData* m_RD, string lvlname);
	std::vector<string> level_names;

	bool new_level = false;
	int level_selected = 0;
};