#include "pch.h"
#include "GameCamera.h"
#include "GameStateData.h"
#include <vector>

bool GameCamera::init(GameStateData* gsd)
{
	base_res_x = gsd->x_resolution;
	base_res_y = gsd->y_resolution;
	x_zoom_resolution = base_res_x;
	y_zoom_resolution = base_res_y;
	x_zoom_bg_resolution = base_res_x;
	y_zoom_bg_resolution = base_res_y;
	
	viewport = { -1.f, -1.f,
		static_cast<float>(base_res_x), static_cast<float>(base_res_y),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };

	background_viewport = { -1.f, -1.f,
		static_cast<float>(base_res_x), static_cast<float>(base_res_y),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };

	UI_viewport = { -1.f, -1.f,
		static_cast<float>(base_res_x), static_cast<float>(base_res_y),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };

	return true;
}

void GameCamera::UpdateCamera(std::vector<std::unique_ptr<Player2D>> &players)
{

	Vector2 centre = CalculateCentre(players);
	Rectangle rect = CalculateCameraRect(players);


	// Calculate zoom
	float xZoomReq = (float)rect.width / x_zoom_resolution;
	float yZoomReq = (float)rect.height / y_zoom_resolution;
	float zoom = 0;

	if (xZoomReq > yZoomReq) { zoom = xZoomReq; }
	else { zoom = yZoomReq; }

	if (zoom == 0) { zoom = 1; }

	y_zoom_resolution *= zoom;
	x_zoom_resolution *= zoom;

	x_zoom_bg_resolution *= zoom / 2;
	y_zoom_bg_resolution *= zoom / 2;

	//Camera Zoom Limits
	{
		//min
		if (x_zoom_resolution < base_res_x*0.75f) { x_zoom_resolution = base_res_x * 0.75f; }
		if (y_zoom_resolution < base_res_y*0.75f) { y_zoom_resolution = base_res_y * 0.75f; }
		//max
		if (x_zoom_resolution > base_res_x*1.5f) { x_zoom_resolution = base_res_x * 1.5f; }
		if (y_zoom_resolution > base_res_y*1.5f) { y_zoom_resolution = base_res_y * 1.5f; }

		//Background Zoom Limits
		//min
		if (x_zoom_bg_resolution < base_res_x*0.5f) { x_zoom_bg_resolution = base_res_x * 0.5f; }
		if (y_zoom_bg_resolution < base_res_y*0.5f) { y_zoom_bg_resolution = base_res_y * 0.5f; }
		//max
		if (x_zoom_bg_resolution > base_res_x*0.8125f) { x_zoom_bg_resolution = base_res_x * 0.8125f; }
		if (y_zoom_bg_resolution > base_res_y*0.8125f) { y_zoom_bg_resolution = base_res_y * 0.8125f; }
	}

	// Viewport position
	top_left_x = -(centre.x * (2.f / x_zoom_resolution) - 1);
	top_left_y = -(centre.y * (2.f / y_zoom_resolution) - 1);

	//if (top_left_y > 0.5){top_left_y = 0.5;}
	//if (top_left_y < -0.5){top_left_y = -0.5;}
	//if (top_left_x > 0.5) { top_left_x = 0.5; }
	//if (top_left_x < -0.3) { top_left_x = -0.3; }

	// update viewports
	viewport = { -1 + top_left_x, -1 + top_left_y,
		static_cast<float>(x_zoom_resolution), static_cast<float>(y_zoom_resolution),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };

	background_viewport = { -2.f + (top_left_x / 4), -2.f + (top_left_y / 4),
		static_cast<float>(x_zoom_bg_resolution), static_cast<float>(y_zoom_bg_resolution),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
}

Rectangle GameCamera::CalculateCameraRect(std::vector<std::unique_ptr<Player2D>> &players)
{
	Rectangle rect;
	// Set base position for rectangle
	for (int i = players.size()-1; i >= 0; i--)
	{
		if (!players[i]->getDead())
		{
			rect = Rectangle(players[i]->GetPos().x, players[i]->GetPos().y, 0, 0);
			break;
		}
	}

	// calculate rectangle from players positions 
	for (auto& player : players)
	{
		if (!player->getDead())
		{
			float x = (int)player->GetPos().x;
			float y = (int)player->GetPos().y;

			if (x < rect.x) { rect.x = x; rect.width += rect.x - x; }
			if (x >= (rect.x + rect.width)) { rect.width = x - rect.x; }

			if (y < rect.y) { rect.y = y; rect.height += rect.y - y; }
			if (y >= (rect.y + rect.height)) { rect.height = y - rect.y; }
		}
	}
	
	// offset rectangle for better results
	rect.x -= 100;
	rect.y -= 100;
	rect.width += 400;
	rect.height += 400;

	
	return rect;
}

Vector2 GameCamera::CalculateCentre(std::vector<std::unique_ptr<Player2D>> &players)
{
	Vector2 position;	
	int j = 0;
	for (auto& player : players)
	{
		if (!player->getDead())
		{
			position += player->GetPos();
			j++;
		}
	}
	return Vector2(position.x / j, position.y / j);
}
