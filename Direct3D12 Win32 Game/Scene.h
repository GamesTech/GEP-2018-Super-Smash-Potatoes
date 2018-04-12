#pragma once
#include "AudioManager.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	void virtual init(RenderData* m_RD, GameStateData* gsd, AudioManager* am) {};
	void virtual update(GameStateData* gsd) {};
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) {};
	void virtual ReadInput(GameStateData* gsd) {};
	void virtual giveSwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain) {};

	GameObject2D& getObjectByIndex(int index) const;
	GameObject2D& getObjectByTag(std::string tag) const;

protected:
	std::vector<GameObject2D*> game_objects;
	AudioManager* audio_manager = nullptr;
};