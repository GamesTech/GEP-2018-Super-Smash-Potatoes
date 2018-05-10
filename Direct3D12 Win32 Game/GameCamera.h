#include "GameObject2D.h"
#include "Player2D.h"

class GameCamera
{
public:
	GameCamera() = default;
	~GameCamera() = default;
	bool init(GameStateData* gsd);
	void UpdateCamera(std::vector<std::unique_ptr<Player2D>> &players);
	D3D12_VIEWPORT getGameViewport() { return viewport; };
	D3D12_VIEWPORT getBGViewport() { return background_viewport; };
	D3D12_VIEWPORT getUIViewport() { return UI_viewport; };
	

private:
	Rectangle CalculateCameraRect(std::vector<std::unique_ptr<Player2D>> &players);

	Vector2 CalculateCentre(std::vector<std::unique_ptr<Player2D>> &players);

	float top_left_x = 0;
	float top_left_y = 0;

	D3D12_VIEWPORT viewport;
	float x_zoom_resolution = 0;
	float y_zoom_resolution = 0;

	D3D12_VIEWPORT background_viewport;
	float x_zoom_bg_resolution = 0;
	float y_zoom_bg_resolution = 0;

	D3D12_VIEWPORT UI_viewport;
	float base_res_x = 0;
	float base_res_y = 0;
};
