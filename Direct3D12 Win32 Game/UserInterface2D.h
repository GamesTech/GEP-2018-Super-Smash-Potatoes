#pragma once
#include "pch.h"
#include "RenderData.h"
#include "GameObject2D.h"

class UserInterface
{
public:
	UserInterface() = default;
	~UserInterface() = default;

	void init(RenderData* m_RD, GameStateData* gsd, std::unique_ptr<Player2D> players[], std::vector<string> sprite_names);
	void update(GameStateData* gsd, std::unique_ptr<Player2D> players[], float time_remaining);
	void render(RenderData* m_RD);

private:
	std::vector<std::unique_ptr<GameObject2D>> UI;

	Text2D* timer_text;
	Text2D* damage_text[4];

	ImageGO2D* player_UI_Boxes;
	ImageGO2D* lives_button_sprite[12];

	int max_lives;
};