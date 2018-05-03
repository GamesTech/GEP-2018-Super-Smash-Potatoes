#include "pch.h"
#include "RenderData.h"
#include "GameStateData.h"
#include "Particle.h"
#include <codecvt>
#include "Emitter.h"


Emitter::Emitter(RenderData * _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer) :ImageGO2D(_RD, _filename, image_buffer)
{
	particles.reserve(50);
}

void Emitter::update(GameStateData * gsd)
{
	for (auto& p : particles) // Updates all the particles that aren't dead
	{
		if (!p->isDead())
		{
			p->update(gsd);
		}
	}
}

void Emitter::render(RenderData * m_RD)
{
	for (auto& p : particles) //For each particle in the it will be rendered as same texture just in a new position
	{
		if (!p->isDead()) // If particle is dead, don't render it.
		{
			FlipH(p->getFlip());
			m_RD->m_spriteBatch->Draw(m_RD->m_resourceDescriptors->GetGpuHandle(m_texture_data.m_resourceNum),
				GetTextureSize(m_texture_data.m_texture.Get()),
				p->getPos(), nullptr, p->getColour(), m_orientation, m_origin, p->getScale(), m_flip, p->getLayer());
		}
	}
}

void Emitter::addBurstOfParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale)
{
	for (int i = 0; i < amount; ++i)
	{
		// Reusing old particles 
		bool made_particle = false;
		for (auto& p : particles)
		{
			if (p->isDead()) // Checks for dead particles, if found they are reused.
			{
				p->init(pos, lifetime, layer, fade, flipH, colour, scale);
				made_particle = true;
				break;
			}
		}
		if (!made_particle) // If no particles can be reuse , create a new one.
		{
			particles.emplace_back(new Particle());
			particles.back()->init(pos, lifetime, layer, fade, flipH, colour, scale);
		}
	}
}

void Emitter::addShootSideParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, Vector2 vel, Vector2 acc)
{
	for (int i = 0; i < amount; ++i)
	{
		// Reusing old particles 
		bool made_particle = false;
		for (auto& p : particles)
		{
			if (p->isDead())// Checks for dead particles, if found they are reused.
			{
				p->init(pos, lifetime, layer, fade, flipH, colour, scale, vel, acc);
				made_particle = true;
				break;
			}
		}
		if (!made_particle) // If no particles can be reuse , create a new one.
		{
			particles.emplace_back(new Particle());
			particles.back()->init(pos, lifetime, layer, fade, flipH, colour, scale, vel, acc);
		}
	}
}
