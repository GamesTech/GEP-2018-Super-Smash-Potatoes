#include "pch.h"
#include "ParticleSystem.h"
#include "RenderData.h"

ParticleSystem::~ParticleSystem()
{
	emitter.clear();
}

bool ParticleSystem::init(RenderData * _m_RD, std::shared_ptr<ImageBuffer> image_buffer)
{
	m_RD = _m_RD;

	loadEmitterFile("emitter.txt");

	for (int i = 0; i < emitter_names.size(); ++i)
	{
		std::unique_ptr<Emitter> emitter_temp = std::make_unique<Emitter>(m_RD, emitter_names.at(i), image_buffer); // Create a new particle 
		emitter.push_back(std::move(emitter_temp));
	}

	return true;
}

void ParticleSystem::update(GameStateData * gsd)
{
	//updates the particles
	for (auto& e : emitter)
	{
		e->update(gsd); // Updates the emiiter till it is empty 
	}
}

void ParticleSystem::render(RenderData * m_RD)
{
	//renders the particles
	for (auto& e : emitter)
	{
		e->render(m_RD);
	}
}


void ParticleSystem::addParticlesToEmitter(int amount, Particle_Type::Type type, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, float x_range, float y_range)
{
	switch (type) // Add the particles to the right emitter for the texture they entered.
	{
	case Particle_Type::DUST:
	{
		emitter[0]->addBurstOfParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale, x_range, y_range);
		break;
	}
	case Particle_Type::ATTACK:
	{
		emitter[1]->addBurstOfParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale, x_range, y_range);
		break;
	}
	case Particle_Type::ATTACK_UPWARDS:
	{
		emitter[2]->addBurstOfParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale, x_range, y_range);
		break;
	}
	case Particle_Type::FIREWORK:
	{
		emitter[3]->addBurstOfParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale, x_range, y_range);
		break;
	}
	}
}

void ParticleSystem::addParticlesToEmitter(int amount, Particle_Type::Type type, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, Vector2 velocity, Vector2 accelaration)
{
	switch (type) // Add the particles to the right emitter for the texture they entered.
	{
	case Particle_Type::DUST:
	{
		emitter[0]->addShootSideParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale, velocity, accelaration);
		break;
	}
	case Particle_Type::ATTACK:
	{
		emitter[1]->addShootSideParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale, velocity, accelaration);
		break;
	}
	case Particle_Type::ATTACK_UPWARDS:
	{
		emitter[2]->addShootSideParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale, velocity, accelaration);
		break;
	}
	case Particle_Type::FIREWORK:
	{
		emitter[3]->addShootSideParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale, velocity, accelaration);
		break;
	}
	}
}

void ParticleSystem::loadEmitterFile(string _filename)
{
	std::ifstream emitter;
	emitter.open(_filename);
	if (emitter.is_open())
	{
		while (!emitter.eof())
		{
			std::string temp_string;
			emitter >> temp_string;
			emitter_names.push_back(temp_string);
		}
	}
	emitter.close();
}