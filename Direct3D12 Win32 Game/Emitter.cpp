#include "pch.h"
#include "RenderData.h"
#include "GameStateData.h"
#include "Particle.h"
#include <codecvt>
#include "Emitter.h"
#include <random>
#include <array>

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
	for (auto& p : particles) //For each particle in the it will be rendered as same texture just in a new position
	{
		m_colour.w = p.opacity;
		FlipH(p.flip);
		m_RD->m_spriteBatch->Draw(m_RD->m_resourceDescriptors->GetGpuHandle(m_resourceNum),
			GetTextureSize(m_texture.Get()),
			p.offset_position + p.position, nullptr, m_colour, m_orientation, m_origin, m_scale, m_flip, p.layer);
	}
}

void Emitter::addBurstOfParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH)
{
	for (int i = 0; i < amount; ++i)
	{
		Particles temp_p;
		temp_p.offset_position = pos;
		normalization(temp_p);
		temp_p.lifetime = lifetime;
		temp_p.fade = fade;
		temp_p.flip = flipH;
		temp_p.layer = layer;
		particles.push_back(temp_p);
	}
}

void Emitter::normalization(Particles& temp_p)
{
	std::random_device rd;
	std::uniform_real_distribution<float> _velocity(-100, 100);
	std::uniform_real_distribution<float> _acc(-100, 100);

	std::array<float, 2> v = { _velocity(rd), -50 + _velocity(rd) };

	float mag = sqrt(v[0] * v[0] + v[1] * v[1]);

	v[0] /= mag;
	v[1] /= mag;

	temp_p.velocity.x = v[0] * _velocity(rd);
	temp_p.velocity.y = -abs(v[1] * _velocity(rd));

	temp_p.accelaration.x = _acc(rd);
	temp_p.accelaration.y = _acc(rd);
}

void Emitter::addShootSideParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Vector2 vel, Vector2 acc)
{
	for (int i = 0; i < amount; ++i)
	{
		Particles temp_p;
		temp_p.offset_position = pos;
		temp_p.lifetime = lifetime;
		temp_p.fade = fade;
		temp_p.flip = flipH;
		temp_p.layer = layer;

		if (flipH)
		{
			temp_p.velocity.x = -vel.x;
			temp_p.velocity.y = vel.y;
			temp_p.accelaration.x = -acc.x;
			temp_p.accelaration.y = acc.y;
		}
		else
		{
			temp_p.velocity.x = vel.x;
			temp_p.velocity.y = vel.y;
			temp_p.accelaration.x = acc.x;
			temp_p.accelaration.y = acc.y;
		}
		particles.push_back(temp_p);
	}
}
