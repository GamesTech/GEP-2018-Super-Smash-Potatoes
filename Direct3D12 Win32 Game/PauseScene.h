#pragma once
#include "Scene.h"

class PauseScene :
	public Scene
{
public:
	// Inherited via Scene
	virtual bool init(RenderData * m_RD, GameStateData * gsd, AudioManager * am, std::shared_ptr<ImageBuffer> image_buffer) override;
	virtual SceneChange update(GameStateData * gsd) override;
	virtual void render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;
	virtual bool renderSceneBelow() { return true; };
	virtual void ReadInput(Input * input_manager) override;

private:
	enum Action
	{
		NONE,
		EXIT
	};

	Action action = Action::NONE;
};