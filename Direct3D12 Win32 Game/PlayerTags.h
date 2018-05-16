#pragma once
#include "pch.h"

class PlayerTags
{
public:
	PlayerTags(int players);
	~PlayerTags() = default;
	void Init(RenderData* m_RD, std::shared_ptr<ImageBuffer> image_buffer);
	void Update();
	void Render(RenderData* m_RD);

	void SetPlayerPos(int player, Vector2 pos, float width);
private:
	int player = 0;
	std::unique_ptr<ImageGO2D> m_player_tag[4];
	RECT m_player_tag_sprite[4] = { { 0,0,32,35 },{ 49,0,95,35 },{ 114,0,160,35 },{ 172,0,218,35 } };
	Vector2 player_pos[4];
	float width[4];
};