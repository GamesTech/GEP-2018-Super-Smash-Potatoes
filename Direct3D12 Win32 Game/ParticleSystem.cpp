#include "pch.h"
#include "ParticleSystem.h"
#include "RenderData.h"

ParticleSystem::~ParticleSystem()
{
	particles.clear();
}

bool ParticleSystem::init(RenderData * _m_RD)
{
	m_RD = _m_RD;

	std::unique_ptr<Emitter> dust_emitter = std::make_unique<Emitter>(m_RD, "dust"); // Create a new particle 
	dust_emitter->init(m_RD); // The type is what particle file is loaded
	emitter.push_back(std::move(dust_emitter));

	std::unique_ptr<Emitter> attack_emitter = std::make_unique<Emitter>(m_RD, "attack"); // Create a new particle 
	attack_emitter->init(m_RD); // The type is what particle file is loaded
	emitter.push_back(std::move(attack_emitter));

	std::unique_ptr<Emitter> attack_upwards_emitter = std::make_unique<Emitter>(m_RD, "upwards_punch_particle"); // Create a new particle 
	attack_upwards_emitter->init(m_RD); // The type is what particle file is loaded
	emitter.push_back(std::move(attack_upwards_emitter));

	return true;
}

void ParticleSystem::update(GameStateData * gsd)
{
	//updates the particles
	for (int i = 0; i < particles.size(); ++i)
	{
		particles[i]->update(gsd);

		if (particles[i]->isDead()) // When the particle are dead they are deleted 
		{
			particles.erase(particles.begin() + i);
		}
	}
	for (auto& e : emitter)
	{
		e->update(gsd); // Updates the emiiter till it is empty 
	}
}

void ParticleSystem::render(RenderData * m_RD)
{
	//renders the particles
	for (auto& p : particles)
	{
		p->render(m_RD);
	}
	for (auto& e : emitter)
	{
		e->render(m_RD);
	}
}

void ParticleSystem::spawnParticle(int amount, Type::Type type, Vector2 pos, bool flipH)
{
	for (int i = 0; i < amount; ++i)
	{
		std::unique_ptr<Particle> p = std::unique_ptr<Particle>(new Particle); // Create a new particle 
		p->init(m_RD, type, pos, flipH); // The type is what particle file is loaded

		particles.push_back(std::move(p));
	}
}

void ParticleSystem::spawnParticle(int amount, Type::Type type, Vector2 pos, bool flipH, Vector2 player_vel)
{
	for (int i = 0; i < amount; ++i)
	{
		std::unique_ptr<Particle> p = std::unique_ptr<Particle>(new Particle); // Create a new particle 
		p->init(m_RD, type, pos, flipH); // The type is what particle file is loaded
		p->setPlayerVel(player_vel);
		particles.push_back(std::move(p));
	}
}

void ParticleSystem::addParticlesToEmitter(int amount, Particle_Type::Type type, Vector2 pos, float lifetime = 1, float layer = 0.0f, bool fade = true, bool flipH = false)
{
	switch (type)
	{
	case Particle_Type::DUST:
	{
		emitter[0]->addBurstOfParticles(amount, pos, lifetime, layer, fade, flipH);
		break;
	}
	case Particle_Type::ATTACK:
	{
		emitter[1]->addBurstOfParticles(amount, pos, lifetime, layer, fade, flipH);
		break;
	}
	}
}

void ParticleSystem::addParticlesToEmitter(int amount, Particle_Type::Type type, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Vector2 velocity, Vector2 accelaration)
{
	switch (type)
	{
	case Particle_Type::DUST:
	{
		emitter[0]->addShootSideParticles(amount, pos, lifetime, layer, fade, flipH, velocity, accelaration);
		break;
	}
	case Particle_Type::ATTACK:
	{
		emitter[1]->addShootSideParticles(amount, pos, lifetime, layer, fade, flipH, velocity, accelaration);
		break;
	}
	case Particle_Type::ATTACK_UPWARDS:
	{
		emitter[2]->addShootSideParticles(amount, pos, lifetime, layer, fade, flipH, velocity, accelaration);
		break;
	}
	}
}

