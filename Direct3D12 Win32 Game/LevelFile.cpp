#include "pch.h"
#include "LevelFile.h"

void LevelFile::read(std::string file, std::string ext)
{
	level_file = std::make_unique<File>(file, ext, true);

	level_file->read();

	/*
		Read In order of:

		Vector2 position;
		Vector2 origin;
		Vector2 scale;
		float orientation;
		float layer;
		string image_file;
	*/
	
	for (int i = 0; i < level_file->getNumberOfBlocks(); i++) 
	{
		auto lines = level_file->getBlock(i);
		GameObjectData obj;
		
		obj.position = File::parseVector2(lines.at(0));
		obj.origin = File::parseVector2(lines.at(1));
		obj.scale = File::parseVector2(lines.at(2));
		obj.orientation = std::stof(lines.at(3));
		obj.layer = std::stof(lines.at(4));
		obj.sprite_size_min = File::parseVector2(lines.at(5));
		obj.sprite_size_max = File::parseVector2(lines.at(6));
		obj.image_file = lines.at(7);

		object_data.push_back(obj);
	}
}
