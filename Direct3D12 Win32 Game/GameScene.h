//#pragma once
#include "Scene.h"
#include "LevelFile.h"

class GameScene : public Scene
{
public:

	GameScene() = default;
	virtual ~GameScene();

	void virtual init(RenderData* m_RD, GameStateData* gsd) override;
	void virtual update(GameStateData* gsd) override;
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;

private:
	virtual void ReadInput(GameStateData* gsd) override;
	void spawnPlayers(GameStateData* gsd, RenderData* m_RD, int no_players);
	void loadCharactersFile(string _filename);
	void CheckAttackPos(GameStateData * _GSD, int _i);
	bool CheckCollision(GameObject2D * _obj, int _i);

	int no_players = 0;

	float time_remaining = 0;

	std::unique_ptr<Player2D> m_player[4];
	std::unique_ptr <LevelFile> level;

	std::vector<string> sprite_names;

	std::vector<std::unique_ptr<GameObject2D>> platforms;
	std::vector<std::unique_ptr<GameObject2D>> objects;
	//Player2D* m_player;
	bool m_anim_grounded[4] = { false,false,false,false };
	Text2D* timer_text;
	Text2D* damage_text;
	std::unique_ptr<ImageGO2D> platform_side;

	ImageGO2D* player_UI_Boxes;
};

