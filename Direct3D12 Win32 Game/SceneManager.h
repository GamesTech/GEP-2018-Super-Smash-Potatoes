#pragma once
#include <stack>
#include "Scene.h"

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

	void init(RenderData* m_RD, GameStateData* gsd, AudioManager* am);
	bool update(RenderData* m_RD, GameStateData* gsd, AudioManager* am, Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain);
	void render(RenderData* m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList);


private:

	std::vector<std::unique_ptr<Scene>> scenes;
};

