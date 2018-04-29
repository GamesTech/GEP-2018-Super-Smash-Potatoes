#include "pch.h"
#include "Particle.h"
#include "GameStateData.h"

void Particle::init(RenderData * m_RD, std::string type, Vector2 pos, bool flipH)
{
	particle_file = std::make_unique<ParticleFile>(); //loads the type of particle it is
	particle_file->read(type, ".particle"); 
	
	sprite = std::make_unique<ImageGO2D>(m_RD, particle_file->getObj(0).image_file); //loads the image from the file
	sprite->SetRect(particle_file->getObj(0).x_min_max.x, particle_file->getObj(0).y_min_max.x, particle_file->getObj(0).x_min_max.y, particle_file->getObj(0).y_min_max.y); //Set the rect of image for rendering
	sprite->SetPos(pos); //set the position
	sprite->SetLayer(particle_file->getObj(0).layer);
	sprite->FlipH(flipH);

	position = pos;
	velocity = particle_file->getObj(0).velocity; // velocity loaded from file
	accelaration = particle_file->getObj(0).accelaration; // accelaration loaded from file too
	lifetime = particle_file->getObj(0).lifetime;
}

void Particle::update(GameStateData* gsd)
{
	lifetime -= gsd->m_dt; //reduces lifetime by deltatime, if hit zero it equals dead
	//Velocity and accelaration maths
	velocity.x = ( velocity.x + accelaration.x * gsd->m_dt);
	velocity.y = (velocity.y + accelaration.y * gsd->m_dt);
	position.x = (position.x + velocity.x * gsd->m_dt);
	position.y = (position.y + velocity.y * gsd->m_dt);
	//updates position
	sprite->SetPos(position);
}

void Particle::render(RenderData * m_RD)
{
	sprite->Render(m_RD);
}
