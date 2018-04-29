#include "pch.h"
#include "Particle.h"
#include "GameStateData.h"

void Particle::init(RenderData * m_RD, std::string type, Vector2 pos)
{
	particle_file = std::make_unique<ParticleFile>();
	particle_file->read(type, ".particle");
	
	sprite = std::make_unique<ImageGO2D>(m_RD, particle_file->getObj(0).image_file);
	sprite->SetPos(pos);
	sprite->SetLayer(particle_file->getObj(0).layer);

	position = pos;
	velocity = particle_file->getObj(0).velocity;
	accelaration = particle_file->getObj(0).accelaration;
	lifetime = particle_file->getObj(0).lifetime;
}

void Particle::update(GameStateData* gsd)
{
	lifetime -= gsd->m_dt;
	velocity.x = ( velocity.x + accelaration.x * gsd->m_dt);
	velocity.y = (velocity.y + accelaration.y * gsd->m_dt);
	position.x = (position.x + velocity.x * gsd->m_dt);
	position.y = (position.y + velocity.y * gsd->m_dt);
}

void Particle::render(RenderData * m_RD)
{
	sprite->Render(m_RD);
}
