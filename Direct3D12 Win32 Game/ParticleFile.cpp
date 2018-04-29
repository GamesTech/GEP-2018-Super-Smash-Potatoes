#include "pch.h"
#include "ParticleFile.h"

void ParticleFile::read(std::string file, std::string ext)
{
	particle_file = std::make_unique<File>(file, ext, true);

	particle_file->read();

	/*
	Read In order of:

	string image_file
	Vector2 velocity
	Vector2 accelaration
	float lifetime
	float layer
	*/

	for (int i = 0; i < particle_file->getNumberOfBlocks(); i++)
	{
		auto lines = particle_file->getBlock(i);
		ParticleObjectData obj;

		obj.image_file = lines.at(0);
		obj.velocity = File::parseVector2(lines.at(1));
		obj.accelaration = File::parseVector2(lines.at(2));
		obj.lifetime = std::stof(lines.at(3));
		obj.layer = std::stof(lines.at(4));

		object_data.push_back(obj);
	}
}
