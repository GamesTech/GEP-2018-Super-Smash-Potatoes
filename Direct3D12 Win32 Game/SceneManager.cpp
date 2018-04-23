#include "pch.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "SettingsScene.h"
#include "CharacterSelectScene.h"
#include "ArenaSelectScene.h"
#include "GameScene.h"
#include "GameOverScene.h"




void SceneManager::init(RenderData * m_RD, GameStateData * gsd, AudioManager * am)
{
	std::unique_ptr<MenuScene> scene = std::make_unique<MenuScene>(new MenuScene);
	scene->init(m_RD, gsd, am);
	scenes.push_back(std::move(scene));
}

bool SceneManager::update(RenderData* m_RD, GameStateData* gsd, AudioManager* am)
{
	if (scenes.size() == 0)
	{
		return false;
	}

	Scene::SceneChange change = scenes.back()->update(gsd);
	scenes.back()->ReadInput(gsd);

	switch (change.change_type)
	{
		case ChangeType::REMOVE:
		{
			scenes.pop_back();
			return true;
		}
		case ChangeType::REPLACE:
		{
			scenes.pop_back();
			break;
		}
		case ChangeType::REPLACE_ALL:
		{
			int size = scenes.size();

			for (int i = 0; i < size; ++i)
			{
				scenes.pop_back();
			}
			break;
		}
		case ChangeType::EXIT:
		{
			scenes.clear();
			return false;
		}
		case ChangeType::NONE:
		{
			return true;
		}
	}

	std::unique_ptr<Scene> scene = nullptr;

	switch (change.scene)
	{
		case SceneEnum::SceneEnum::MENU:
		{
			scene = std::make_unique<MenuScene>();
			break;
		}

		case SceneEnum::SceneEnum::SETTINGS:
		{
			scene = std::make_unique<SettingsScene>();
			break;
		}

		case SceneEnum::SceneEnum::CHARACTER_SELECTION:
		{
			scene = std::make_unique<CharacterSelectScene>();
			break;
		}

		case SceneEnum::SceneEnum::ARENA_SELECTION:
		{
			scene = std::make_unique<ArenaSelectScene>();
			break;
		}

		case SceneEnum::SceneEnum::GAME:
		{
			scene = std::make_unique<GameScene>();
			break;
		}

		case SceneEnum::SceneEnum::GAMEOVER:
		{
			scene = std::make_unique<GameOverScene>();
			break;
		}
	}
	if (!scene->init(m_RD, gsd, am))
	{
		return false;
	}

	scenes.push_back(std::move(scene));

	return true;
}

void SceneManager::render(RenderData * m_RD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList)
{
	if (scenes.size() > 0)
	{
		scenes.back()->render(m_RD, m_commandList);
	}
}


