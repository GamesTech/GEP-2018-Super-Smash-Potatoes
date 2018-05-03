#include "pch.h"
#include "ParticleSystem.h"
#include "RenderData.h"

ParticleSystem::~ParticleSystem()
{
	emitter.clear();
}

bool ParticleSystem::init(RenderData * _m_RD)
{
	m_RD = _m_RD;

	std::unique_ptr<Emitter> dust_emitter = std::make_unique<Emitter>(m_RD, "dust"); // Create a new particle 
	emitter.push_back(std::move(dust_emitter));

	std::unique_ptr<Emitter> attack_emitter = std::make_unique<Emitter>(m_RD, "attack"); // Create a new particle 
	emitter.push_back(std::move(attack_emitter));

	std::unique_ptr<Emitter> attack_upwards_emitter = std::make_unique<Emitter>(m_RD, "upwards_punch_particle"); // Create a new particle 
	emitter.push_back(std::move(attack_upwards_emitter));

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


void ParticleSystem::addParticlesToEmitter(int amount, Particle_Type::Type type, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale)
{
	switch (type) // Add the particles to the right emitter for the texture they entered.
	{
	case Particle_Type::DUST:
	{
		emitter[0]->addBurstOfParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale);
		break;
	}
	case Particle_Type::ATTACK:
	{
		emitter[1]->addBurstOfParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale);
		break;
	}
	case Particle_Type::ATTACK_UPWARDS:
	{
		emitter[2]->addBurstOfParticles(amount, pos, lifetime, layer, fade, flipH, colour, scale);
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
	}
}

