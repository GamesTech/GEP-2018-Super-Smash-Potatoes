#pragma once
#include <stack>
#include "Scene.h"

class SceneManager
{
public:
	static SceneManager & getInstance();

	SceneManager() = default;
	~SceneManager();

	void init();
	void update();

	
	//Adds a scene to the top of the stack
	void pushScene(const Scene &scene);

	//Will pop the current scene from the stack
	void popScene();

private:

	static SceneManager* instance;

	std::stack<Scene> scene_stack;
};

