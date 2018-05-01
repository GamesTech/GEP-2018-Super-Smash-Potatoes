#include "pch.h"
#include "RenderData.h"
#include "GameStateData.h"
#include "Particle.h"
#include <codecvt>
#include "Emitter.h"


Emitter::Emitter(RenderData * _RD, string _filename) :ImageGO2D(_RD, _filename)
{
}

void Emitter::init(RenderData * m_RD)
{
}

void Emitter::update(GameStateData * gsd)
{
	for (int i = 0; i < particles.size(); ++i)
	{
		particles[i]->update(gsd);
		if (particles[i]->isDead()) // When the particle are dead they are deleted 
		{
			particles.erase(particles.begin() + i);
		}
	}
}

void Emitter::render(RenderData * m_RD)
{
	for (auto& p : particles) //For each particle in the it will be rendered as same texture just in a new position
	{
		FlipH(p->getFlip());
		m_RD->m_spriteBatch->Draw(m_RD->m_resourceDescriptors->GetGpuHandle(m_resourceNum),
			GetTextureSize(m_texture.Get()),
			p->getPos(), nullptr, p->getColour(), m_orientation, m_origin, p->getScale(), m_flip, p->getLayer());
	}
}

void Emitter::addBurstOfParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale)
{
	for (int i = 0; i < amount; ++i)
	{
		particles.emplace_back(new Particle(pos, lifetime, layer, fade, flipH, colour, scale));
		particles.back()->init();
	}
}

void Emitter::normalization(Particles& temp_p)
{
	
}

void Emitter::addShootSideParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, Vector2 vel, Vector2 acc)
{
	for (int i = 0; i < amount; ++i)
	{
		particles.emplace_back(new Particle(pos, lifetime, layer, fade, flipH, colour, scale));
		particles.back()->init(vel, acc);
	}
}
