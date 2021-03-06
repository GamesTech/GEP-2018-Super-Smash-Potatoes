//#pragma once
#include "Scene.h"
#include "LevelFile.h"
#include "PlayerTags.h"
#include "UserInterface2D.h"
#include "ParticleSystem.h"
#include "CollisionSystem.h"
#include "ItemSpawner.h"
#include "GameCamera.h"

class GameScene : public Scene
{
public:

	GameScene() = default;
	virtual ~GameScene();

	virtual bool init(RenderData* m_RD, GameStateData* gsd, AudioManager* am, std::shared_ptr<ImageBuffer> ib) override;
	virtual SceneChange update(GameStateData* gsd) override;
	void endGame(int players_dead, GameStateData * gsd);
	void Attacking(Player2D* _player, GameStateData * gsd);
	virtual void render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) override;

private:
	enum Action
	{
		NONE,
		BACK,
		CONTINUE,
		PAUSE
	};
	Action action = Action::NONE;

	virtual void ReadInput(Input* input_manager) override;
	void spawnPlayers(GameStateData* gsd, RenderData* m_RD, int no_players);
	void loadCharactersFile(string _filename);

	int no_players = 0;

	float time_remaining = 0;
	float item_spawn_timer = 0;
	int max_lives = 0;
	double timer = 0;
	bool gameEnded = false;

	std::shared_ptr<ParticleSystem> particle_system = nullptr;

	std::vector<std::unique_ptr<Player2D>> m_players;
	Vector2 m_spawn_pos[4] = { {300, 250},{400, 250},{ 600, 250 },{ 700, 250 } };
	std::unique_ptr<PlayerTags> m_player_tag = nullptr;
	std::unique_ptr <LevelFile> level;
	std::unique_ptr <UserInterface> UI;
	std::unique_ptr <GameCamera> m_camera;

	std::vector<string> sprite_names;

	std::vector<std::unique_ptr<GameObject2D>> platforms;
	std::vector<std::unique_ptr<GameObject2D>> objects;
	std::unique_ptr<ItemSpawner> spawner;
	bool m_anim_grounded[4] = { false,false,false,false };
	Text2D* timer_text;
	Text2D* damage_text[4];
	ImageGO2D* lives_button_sprite[12];
	std::unique_ptr<ImageGO2D> platform_side;
	std::unique_ptr<CollisionSystem> m_collision_system = nullptr;

	Input* input_manager;
};

