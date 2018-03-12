#pragma once
#include "File.h"

struct GameObjectData {
	Vector2 position;
	Vector2 origin;
	Vector2 scale;
	float orientation;
	float layer;
	string image_file;
};

class LevelFile
{
public:
	LevelFile() = default;
	~LevelFile() = default;

	void read(std::string file, std::string ext);
	GameObjectData getObj(int index) { return object_data.at(index); };
	int getObjListSize() { return object_data.size(); };

private:
	std::unique_ptr<File> level_file;
	std::vector<GameObjectData> object_data;
};

