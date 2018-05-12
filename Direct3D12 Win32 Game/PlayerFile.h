#pragma once
#pragma once
#include "pch.h"
#include "File.h"
#include "Input.h"

struct PlayerObjectData
{
	float jump_power;
	float upwards_punch_jump_power;
	std::string animation_sheet;
	float drive;
	float drag;
	Inputs::Inputs item_throw_control;
	Inputs::Inputs jump_control;
	Inputs::Inputs jump_down_control;
	Inputs::Inputs Upwards_attack_control;
	Inputs::Inputs slam_control;
	Inputs::Inputs attack_control;
};

class PlayerFile
{
public:
	PlayerFile() = default;
	~PlayerFile() = default;

	void read(std::string file, std::string ext);
	PlayerObjectData getObj(int index) { return object_data.at(index); };
	int getObjListSize() { return object_data.size(); };

private:
	Inputs::Inputs inputConver(std::string input);

	std::unique_ptr<File> player_file;
	std::vector<PlayerObjectData> object_data;
	std::string buttons[14] = { "UP","DOWN","LEFT","RIGHT","A","X", "B","Y","START","LB","RB","DOWN_A","UP_X","DOWN_X" };
};