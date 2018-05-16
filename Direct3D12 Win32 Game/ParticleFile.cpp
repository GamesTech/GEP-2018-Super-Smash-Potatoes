#include "pch.h"
#include "ParticleFile.h"

void ParticleFile::read(std::string file, std::string ext)
{
	particle_file = std::make_unique<File>(file, ext, true);

	particle_file->read();

	/*
	Read In order of:

	string image_file
	float lifetime
	int type_of_particle
	Vector2 velocity
	Vector2 accelaration 
	Vector2 burst_particle_range
	float layer
	bool fade
	float opacity
	bool increase_in_size
	float scale
	bool random_colour 
	*/

	for (int i = 0; i < particle_file->getNumberOfBlocks(); i++)
	{
		auto lines = particle_file->getBlock(i);
		ParticleObjectData obj;

		obj.image_file = lines.at(0);
		obj.lifetime = std::stof(lines.at(1));
		obj.type_of_particle = std::stof(lines.at(2));
		obj.velocity = File::parseVector2(lines.at(3));
		obj.accelaration = File::parseVector2(lines.at(4));
		obj.burst_particle_range = File::parseVector2(lines.at(5));
		obj.layer = std::stof(lines.at(6));
		obj.fade = File::parseBoolean(lines.at(7));
		obj.opacity = std::stof(lines.at(8));
		obj.increase_in_size = File::parseBoolean(lines.at(9));
		obj.scale = std::stof(lines.at(10));
		obj.random_colour = File::parseBoolean(lines.at(11));
		object_data.push_back(obj);
	}
}
