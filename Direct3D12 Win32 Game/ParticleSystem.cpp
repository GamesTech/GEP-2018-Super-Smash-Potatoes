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
	for (int i = 0; i < emitter.size(); ++i)
	{
		if (emitter[i]->update(gsd)) // Updates the emiiter till it is empty 
		{
			emitter.erase(emitter.begin() + i);
		}
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

void ParticleSystem::createEmitter(int amount, std::string image_name, Vector2 pos, bool flipH)
{
	std::unique_ptr<Emitter> e = std::make_unique<Emitter>(m_RD, image_name); // Create a new particle 
	e->init(m_RD, amount, pos, flipH); // The type is what particle file is loaded
	emitter.push_back(std::move(e));
}
