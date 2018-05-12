#include "pch.h"
#include "PlayerFile.h"

void PlayerFile::read(std::string file, std::string ext)
{
	player_file = std::make_unique<File>(file, ext, true);

	player_file->read();

	/*
	Read In order of:

	float jump_power;
	float upwards_punch_jump_power;
	std::string animation_sheet;
	float drive;
	float drag;
	int item_throw_control;
	int jump_control;
	int jump_down_control;
	int Upwards_attack_control;
	int slam_control;
	int attack_control;
	*/

	for (int i = 0; i < player_file->getNumberOfBlocks(); i++)
	{
		auto lines = player_file->getBlock(i);
		PlayerObjectData obj;

		obj.jump_power = std::stof(lines.at(0));
		obj.upwards_punch_jump_power = std::stof(lines.at(1));
		obj.animation_sheet = lines.at(2);
		obj.drive = std::stof(lines.at(3));
		obj.drag = std::stof(lines.at(4));

		obj.item_throw_control = inputConver(lines.at(5));
		obj.jump_control = inputConver(lines.at(6));
		obj.jump_down_control = inputConver(lines.at(7));
		obj.Upwards_attack_control = inputConver(lines.at(8));
		obj.slam_control = inputConver(lines.at(9));
		obj.attack_control = inputConver(lines.at(10));

		object_data.push_back(obj);
	}
}

Inputs::Inputs PlayerFile::inputConver(std::string input)
{
	for (int i = 0; i < 14; ++i)
	{
		if (buttons[i] == input)
		{
			return (Inputs::Inputs) i;
		}
	}
}
