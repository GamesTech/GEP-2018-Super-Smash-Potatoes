#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:

	GameScene();
	~GameScene();

	void virtual init() override;
	void virtual update() override;
	void virtual render() override;

private:

};

