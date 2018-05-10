#pragma once
#include "pch.h"
#include "Particle.h"
#include "Emitter.h"

namespace Particle_Type
{
	enum Type
	{
		ATTACK,
		ATTACK_UPWARDS,
		DUST,
		FIREWORK
	};
}

class ParticleSystem
{
public:
	ParticleSystem() = default;
	~ParticleSystem();
	bool init(RenderData* m_RD, std::shared_ptr<ImageBuffer> image_buffer);
	void update(GameStateData* gsd);
	void render(RenderData* m_RD);

	void addParticles(int amount, Particle_Type::Type type, Vector2 pos, bool flipH);

	/*  Amount of particles created,
	Particle type by enum,
	Position of where they spawn,
	Lifetime of the particle,
	The layer which the particle is rendered,
	Particle fade over time,
	Flip the image horizontal,
	Colour of the image,
	Scale of the image,
	X range of particles,
	Y range of particles*/
	//void addParticlesToEmitter(int amount, Particle_Type::Type type, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, float x_range, float y_range);

	/*  Amount of particles created,
	Particle type by enum,
	Position of where they spawn,
	Lifetime of the particle,
	The layer which the particle is rendered,
	Particle fade over time,
	Flip the image horizontal,
	Colour of the image {R,B,G,Alpha},
	Scale of the image,
	Velocity of the particles,
	Accelaration of the particles*/
	//void addParticlesToEmitter(int amount, Particle_Type::Type type, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, Vector2 velocity, Vector2 accelaration);

private:
	RenderData * m_RD = nullptr;
	std::vector<std::unique_ptr<Emitter>> emitter;
	std::vector<std::string> emitter_names;
	void loadEmitterFile(string _filename);
};