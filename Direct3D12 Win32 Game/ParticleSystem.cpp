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

	loadEmitterFile("Particles\\emitter.txt");

	for (int i = 0; i < emitter_names.size(); ++i)
	{
		std::unique_ptr<Emitter> emitter_temp = std::make_unique<Emitter>(m_RD, emitter_names.at(i), image_buffer, "Particles\\" + emitter_names.at(i)); // Create a new particle 
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

void ParticleSystem::addParticles(int amount, Particle_Type::Type type, Vector2 pos, bool flipH)
{
	// Add the particles to the right emitter for the texture they entered.
	emitter[type]->addParticles(amount, pos, flipH);
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