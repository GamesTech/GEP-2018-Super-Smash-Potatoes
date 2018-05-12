#include "pch.h"
#include "RenderData.h"
#include "GameStateData.h"
#include "Particle.h"
#include <codecvt>
#include "Emitter.h"


Emitter::Emitter(RenderData * _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer, std::string particle) :ImageGO2D(_RD, _filename, image_buffer)
{
	particles.reserve(50);
	particle_file = std::make_shared<ParticleFile>();
	particle_file->read(particle, ".txt");
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

void Emitter::addParticles(int amount, Vector2 pos, bool flipH)
{
	for (int i = 0; i < amount; ++i)
	{
		// Reusing old particles 
		bool made_particle = false;
		for (auto& p : particles)
		{
			if (p->isDead()) // Checks for dead particles, if found they are reused.
			{
				p->SetVariables(pos, flipH);
				made_particle = true;
				break;
			}
		}
		if (!made_particle) // If no particles can be reuse , create a new one.
		{
			particles.emplace_back(new Particle());
			particles.back()->init(particle_file, pos, flipH);
		}
	}
}
