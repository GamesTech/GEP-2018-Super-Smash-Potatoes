#pragma once
#include "pch.h"
#include "File.h"

struct ParticleObjectData 
{
	string image_file;
	Vector2 velocity = { 0, 0 };
	Vector2 accelaration = { 0, 0 };
	float lifetime = 0;
	float layer = 0;
	Vector2 x_min_max;
	Vector2 y_min_max;
};

class ParticleFile
{
public:
	ParticleFile() = default;
	~ParticleFile() = default;

	void read(std::string file, std::string ext);
	ParticleObjectData getObj(int index) { return object_data.at(index); };
	int getObjListSize() { return object_data.size(); };

private:
	std::unique_ptr<File> particle_file;
	std::vector<ParticleObjectData> object_data;
};