#pragma once
#include "File.h"

struct GameObjectData {
	Vector2 position;
	Vector2 origin;
	Vector2 scale;
	float orientation;
	float layer;
	int type;
	Vector2 sprite_size_min;
	Vector2 sprite_size_max;
	string image_file;
};

class LevelFile
{
public:
	LevelFile() = default;
	~LevelFile() = default;

	void read(std::string file, std::string ext);
	void write(std::string file, std::string ext);
	void objectToWrite(GameObjectData object) { object_data_write.push_back(object); };
	GameObjectData getObj(int index) { return object_data.at(index); };
	int getObjListSize() { return object_data.size(); };

private:
	std::unique_ptr<File> level_file_read;
	std::unique_ptr<File> level_file_write;
	std::vector<GameObjectData> object_data;
	std::vector<GameObjectData> object_data_write;
};

