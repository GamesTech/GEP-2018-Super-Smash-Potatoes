#include "pch.h"
#include "ParticleSystem.h"
#include "RenderData.h"

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
			m_RD->m_resourceCount--;
			particles.erase(particles.begin() + i);
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
