#include "pch.h"
#include "SceneManager.h"
#include "StartScene.h"
#include "MenuScene.h"
#include "SettingsScene.h"
#include "CharacterSelectScene.h"
#include "ArenaSelectScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "LevelEditorScene.h"
#include "GameStateData.h"

void SceneManager::init(RenderData * m_RD, GameStateData * gsd, AudioManager * am)
{
	image_buffer = std::make_shared<ImageBuffer>();
	std::unique_ptr<Scene> scene = std::make_unique<StartScene>();
	scene->init(m_RD, gsd, am, image_buffer);
	scenes.push_back(std::move(scene));
}

bool SceneManager::update(RenderData* m_RD, GameStateData* gsd, AudioManager* am, Input* im, Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain)
{
	if (scenes.size() == 0)
	{
		return false;
	}

	Scene::SceneChange change = scenes.back()->update(gsd);
	scenes.back()->ReadInput(im);
	if (im->clearInput())
	{
		timer = 0;
	}
	timer += gsd->m_dt;
	if (timer > 30)
	{
		change.change_type = ChangeType::REPLACE_ALL;
		change.scene = SceneEnum::START;
		timer = 0;
	}

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
		case ChangeType::REPLACE_ALL_BUT_ONE:
		{
			int size = scenes.size() - 1;

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
		case SceneEnum::SceneEnum::START:
		{
			scene = std::make_unique<StartScene>();
			break;
		}

		case SceneEnum::SceneEnum::MENU:
		{
			scene = std::make_unique<MenuScene>();
			break;
		}

		case SceneEnum::SceneEnum::SETTINGS:
		{
			scene = std::make_unique<SettingsScene>();
			scene->giveSwapChain(swapChain);
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
		case SceneEnum::SceneEnum::LEVEL_EDITOR:
		{
			scene = std::make_unique<LevelEditor>();
			break;
		}
	}
	if (!scene->init(m_RD, gsd, am, image_buffer))
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