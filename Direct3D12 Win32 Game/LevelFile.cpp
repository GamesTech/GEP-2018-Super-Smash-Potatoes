#include "pch.h"
#include "LevelFile.h"

void LevelFile::read(std::string file, std::string ext)
{
	level_file_read = std::make_unique<File>(file, ext, true);

	level_file_read->read();

	/*
		Read In order of:
		string image_file;
		Vector2 position;
		Vector2 origin;
		Vector2 scale;
		float orientation;
		float layer;
		int type;
		Vector2 sprite min x & y
		Vector2 sprite max x & y
		
	*/
	
	for (int i = 0; i < level_file_read->getNumberOfBlocks(); i++)
	{
		auto lines = level_file_read->getBlock(i);
		GameObjectData obj;
		
		obj.image_file = lines.at(0);
		obj.position = File::parseVector2(lines.at(1));
		obj.origin = File::parseVector2(lines.at(2));
		obj.scale = File::parseVector2(lines.at(3));
		obj.orientation = std::stof(lines.at(4));
		obj.layer = std::stof(lines.at(5));
		obj.type = std::stof(lines.at(6));
		obj.sprite_size_min = File::parseVector2(lines.at(7));
		obj.sprite_size_max = File::parseVector2(lines.at(8));

		object_data.push_back(obj);
	}
}

void LevelFile::write(std::string file, std::string ext)
{
	level_file_write = std::make_unique<File>(file, ext, false);

	/*
	Read In order of:
	string image_file;
	Vector2 position;
	Vector2 origin;
	Vector2 scale;
	float orientation;
	float layer;
	int type;
	Vector2 sprite min x & y
	Vector2 sprite max x & y

	*/

	for (int i = 0; i < object_data_write.size(); i++)
	{
		auto obj = object_data_write.at(i);
		
		std::vector<std::string> to_file = { 
			("image name:" + obj.image_file) , 
			("position:" + std::to_string(obj.position.x) + ", " + std::to_string(obj.position.y)),
			("origin:" + std::to_string(obj.origin.x) + ", " + std::to_string(obj.origin.y)),
			("scale:" + std::to_string(obj.scale.x) + ", " + std::to_string(obj.scale.y)),
			("orientation:" + std::to_string(obj.orientation)),
			("layer:" + std::to_string(obj.layer)),
			("type:" + std::to_string(obj.type)),
			("sprite min X & Y:" + std::to_string(obj.sprite_size_min.x) + ", " + std::to_string(obj.sprite_size_min.y)),
			("sprite max X & Y:" + std::to_string(obj.sprite_size_max.x) + ", " + std::to_string(obj.sprite_size_max.y))};

		level_file_write->addBlock(to_file);
	}
	level_file_write->write();
	object_data_write.clear();
}
