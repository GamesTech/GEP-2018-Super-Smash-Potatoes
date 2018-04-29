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
	for (int i = 0; i < particles.size(); ++i)
	{
		particles[i]->update(gsd);

		if (particles[i]->isDead())
		{
			m_RD->m_resourceCount--;
			particles.erase(particles.begin() + i);
		}
	}
}

void ParticleSystem::render(RenderData * m_RD)
{
	for (auto& p : particles)
	{
		p->render(m_RD);
	}
}

void ParticleSystem::spawnParticle(int amount, std::string type, Vector2 pos)
{
	for (int i = 0; i < amount; ++i)
	{
		std::unique_ptr<Particle> p = std::unique_ptr<Particle>(new Particle);
		p->init(m_RD, type, pos);

		particles.push_back(std::move(p));
	}
}
