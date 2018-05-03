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
	virtual void ReadInput(GameStateData* gsd) override;

private:
	enum Action
	{
		NONE,
		START,
		BACK,
		ADD_BLOCK,
		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		RB,
		LB,
		PAUSE,
		B

	};
	Action action = Action::NONE;
	RenderData * m_RD = nullptr;

	std::unique_ptr <LevelFile> level;
	std::vector<std::unique_ptr<GameObject2D>> platforms;

	void createNewObject(int type);
	void moveLastObject(float x, float y);
	void saveLevel();


private:
	int object_type = 1;

};