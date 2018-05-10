#include "pch.h"
#include "Particle.h"
#include "GameStateData.h"
#include <random>
#include <array>


void Particle::init(std::string particle_f, Vector2 pos, bool _flip)
{
	particle_file = std::make_unique<ParticleFile>();
	particle_file->read(particle_f , ".particle");
	flip = _flip;

}

void Particle::init(Vector2 pos, float _lifetime, float _layer, bool _fade, bool _flip, Color _colour, float _scale, float x_range, float y_range)
{
	SetVariables(pos, _lifetime, _layer, _fade, _flip, _colour, _scale);
	std::random_device rd;
	std::uniform_real_distribution<float> x_velocity(-x_range, x_range);
	std::uniform_real_distribution<float> y_velocity(-y_range, y_range);
	std::uniform_real_distribution<float> _acc(-100, 100);

	//Normalises the vectors
	std::array<float, 2> v = { x_velocity(rd), y_velocity(rd) };

	float mag = sqrt(v[0] * v[0] + v[1] * v[1]);

	v[0] /= mag;
	v[1] /= mag;

	velocity.x = v[0] * x_velocity(rd);
	velocity.y = v[1] * y_velocity(rd);

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
	lifetime = particle_file->getObj(0).lifetime;
	max_lifetime = particle_file->getObj(0).lifetime;
	layer = particle_file->getObj(0).layer;
	fade = particle_file->getObj(0).fade;
	scale = particle_file->getObj(0).scale;
	increase_size = particle_file->getObj(0).increase_in_size;
	if (particle_file->getObj(0).type_of_particle == 1)
	{
		burstParticles();
	}
	else if (particle_file->getObj(0).type_of_particle == 2)
	{
		velocity.x = particle_file->getObj(0).velocity.x;
		velocity.y = particle_file->getObj(0).velocity.y;
		accelaration.x = particle_file->getObj(0).accelaration.x;
		accelaration.y = particle_file->getObj(0).accelaration.y;
	}
}

void Particle::burstParticles()
{
	std::random_device rd;
	std::uniform_real_distribution<float> x_velocity(-particle_file->getObj(0).burst_particle_range.x, particle_file->getObj(0).burst_particle_range.x);
	std::uniform_real_distribution<float> y_velocity(-particle_file->getObj(0).burst_particle_range.y, particle_file->getObj(0).burst_particle_range.y);
	std::uniform_real_distribution<float> _acc(-100, 100);

	//Normalises the vectors
	std::array<float, 2> v = { x_velocity(rd), y_velocity(rd) };

	float mag = sqrt(v[0] * v[0] + v[1] * v[1]);

	v[0] /= mag;
	v[1] /= mag;

	velocity.x = v[0] * x_velocity(rd);
	velocity.y = v[1] * y_velocity(rd);

	accelaration.x = _acc(rd);
	accelaration.y = _acc(rd);
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
