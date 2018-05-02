#include "pch.h"
#include "Particle.h"
#include "GameStateData.h"
#include <random>
#include <array>


void Particle::init(Vector2 pos, float _lifetime, float _layer, bool _fade, bool _flip, Color _colour, float _scale)
{
	SetVariables(pos, _lifetime, _layer, _fade, _flip, _colour, _scale);
	std::random_device rd;
	std::uniform_real_distribution<float> _velocity(-100, 100);
	std::uniform_real_distribution<float> _acc(-100, 100);

	//Normalises the vectors
	std::array<float, 2> v = { _velocity(rd), 0 };

	float mag = sqrt(v[0] * v[0] + v[1] * v[1]);

	v[0] /= mag;
	v[1] /= mag;

	velocity.x = v[0] * _velocity(rd);
	velocity.y = -abs(v[1] * _velocity(rd));

	accelaration.x = _acc(rd);
	accelaration.y = _acc(rd);
}

void Particle::init(Vector2 pos, float _lifetime, float _layer, bool _fade, bool _flip, Color _colour, float _scale, Vector2 vel, Vector2 acc)
{
	SetVariables(pos, _lifetime, _layer, _fade, _flip, _colour, _scale);

	if (flip) // flips the direction of the particle
	{
		velocity.x = -vel.x;
		velocity.y = vel.y;
		accelaration.x = -acc.x;
		accelaration.y = acc.y;
	}
	else
	{
		velocity.x = vel.x;
		velocity.y = vel.y;
		accelaration.x = acc.x;
		accelaration.y = acc.y;
	}
}

//set the variables for the particle
void Particle::SetVariables(Vector2 &pos, float _lifetime, float _layer, bool _fade, bool _flip, Color &_colour, float _scale)
{
	dead = false;
	position = Vector2{ 0,0 };
	offset_position = pos;
	lifetime = _lifetime;
	max_lifetime = _lifetime;
	layer = _layer;
	fade = _fade;
	flip = _flip;
	colour = _colour;
	scale = _scale;
}

void Particle::update(GameStateData* gsd)
{
	//reduces lifetime by deltatime, if hit zero it equals dead
	lifetime -= gsd->m_dt; 

	//Velocity and accelaration maths
	velocity.x = (velocity.x + accelaration.x * gsd->m_dt);
	velocity.y = (velocity.y + accelaration.y * gsd->m_dt);
	position.x = (position.x + velocity.x * gsd->m_dt);
	position.y = (position.y + velocity.y * gsd->m_dt);

	if (fade) //Fades the particles
	{
		float lifetime_percentage = (max_lifetime - lifetime) / max_lifetime;
		colour.w = 0.0 * lifetime_percentage + 1 * (1 - lifetime_percentage); 
	}
	if (increase_size) // Increases the scale over time
	{
		float dt = gsd->m_dt;
		scale += dt;
	}
	if (lifetime <= 0.0f) // Particles are set to dead after 
	{
		dead = true;
	}
}
