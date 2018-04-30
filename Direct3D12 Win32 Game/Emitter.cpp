#include "pch.h"
#include "RenderData.h"
#include "GameStateData.h"
#include "Particle.h"
#include <codecvt>
#include "Emitter.h"
#include <random>

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
		particles[i].lifetime -= gsd->m_dt; //reduces lifetime by deltatime, if hit zero it equals dead
		//Velocity and accelaration maths
		particles[i].velocity.x = (particles[i].velocity.x + particles[i].accelaration.x * gsd->m_dt);
		particles[i].velocity.y = (particles[i].velocity.y + particles[i].accelaration.y * gsd->m_dt);
		particles[i].position.x = (particles[i].position.x + particles[i].velocity.x * gsd->m_dt);
		particles[i].position.y = (particles[i].position.y + particles[i].velocity.y * gsd->m_dt);

		if (particles[i].fade)
		{
			particles[i].opacity = 0.2 * (0.5 - particles[i].lifetime) + 1 * particles[i].lifetime;
		}

		if (particles[i].lifetime <= 0.0f) // When the particle are dead they are deleted 
		{
			particles.erase(particles.begin() + i);
		}
	}
}

void Emitter::render(RenderData * m_RD)
{
	for (auto& p : particles)
	{
		m_colour.w = p.opacity;
		FlipH(p.flip);
		m_RD->m_spriteBatch->Draw(m_RD->m_resourceDescriptors->GetGpuHandle(m_resourceNum),
			GetTextureSize(m_texture.Get()),
			p.offset_position + p.position, nullptr, m_colour, m_orientation, m_origin, m_scale, m_flip, p.layer);
	}
}

void Emitter::addParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH)
{
	for (int i = 0; i < amount; ++i)
	{
		std::random_device rd;
		std::uniform_real_distribution<float> _velocity(-100, 100);
		std::uniform_real_distribution<float> _acc(-100, 100);
		Particles temp_p;
		temp_p.offset_position = pos;
		temp_p.velocity.x = _velocity(rd);
		temp_p.velocity.y = -_velocity(rd);
		temp_p.accelaration.x = _acc(rd);
		temp_p.accelaration.y = _acc(rd);
		temp_p.lifetime = lifetime;
		temp_p.fade = fade;
		temp_p.flip = flipH;
		temp_p.layer = layer;
		particles.push_back(temp_p);
	}
}
