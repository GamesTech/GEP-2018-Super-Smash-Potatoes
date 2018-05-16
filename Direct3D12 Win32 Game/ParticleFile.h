#pragma once
#include "pch.h"
#include "File.h"

struct ParticleObjectData 
{
	string image_file;
	float lifetime = 0;
	int type_of_particle = 0;
	Vector2 velocity = { 0, 0 };
	Vector2 accelaration = { 0, 0 };
	Vector2 burst_particle_range = { 0, 0 };
	float layer = 0;
	bool fade = false;
	float opacity = 1.0f;
	bool increase_in_size = false;
	float scale = 1.0f;
	bool random_colour = false;
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