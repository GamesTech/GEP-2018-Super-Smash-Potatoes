#pragma once
#include "AudioManager.h"

namespace ChangeType
{
	enum ChangeType
	{
		REPLACE,
		REPLACE_ALL,
		REMOVE,
		ADD,
		EXIT,
		NONE
	};
}

namespace SceneEnum
{
	enum SceneEnum
	{
		MENU,
		CHARACTER_SELECTION,
		ARENA_SELECTION,
		SETTINGS,
		GAME,
		GAMEOVER,
		LEVEL_EDITOR,
		NONE
	};
}

class Scene
{
public:

	struct SceneChange
	{
		ChangeType::ChangeType change_type = ChangeType::NONE;
		SceneEnum::SceneEnum scene = SceneEnum::NONE;
	};

	Scene() = default;
	virtual ~Scene() = default;

	 virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am) = 0;
	 virtual SceneChange update(GameStateData* gsd) = 0;
	 virtual void render(RenderData* m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) = 0;
	 virtual void ReadInput(GameStateData* gsd) {};
	 virtual void giveSwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain) {};

	GameObject2D& getObjectByIndex(int index) const;
	GameObject2D& getObjectByTag(std::string tag) const;

protected:
	std::vector<std::unique_ptr<GameObject2D>> game_objects;
	AudioManager* audio_manager = nullptr;
	int no_players;
};