#include "pch.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance;

SceneManager::~SceneManager()
{
	delete instance;
}

void SceneManager::init()
{
	
}

void SceneManager::update()
{
}

void SceneManager::pushScene(const Scene &scene)
{
	scene_stack.push(scene);
}

void SceneManager::popScene()
{
	scene_stack.pop();
}

SceneManager & SceneManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new SceneManager();
	}

	return *instance;
}
