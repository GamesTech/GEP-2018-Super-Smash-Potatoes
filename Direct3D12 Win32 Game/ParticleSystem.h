#pragma once
#include "pch.h"
#include "Particle.h"
#include "Emitter.h"

namespace Particle_Type
{
	enum Type
	{
		DUST,
		ATTACK,
		ATTACK_UPWARDS
	};
}

class ParticleSystem
{
public:
	ParticleSystem() = default;
	~ParticleSystem();
	bool init(RenderData* m_RD);
	void update(GameStateData* gsd);
	void render(RenderData* m_RD);

	void addParticlesToEmitter(int amount, Particle_Type::Type type, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale);
	void addParticlesToEmitter(int amount, Particle_Type::Type type, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, Vector2 velocity, Vector2 accelaration);

private:
	RenderData * m_RD = nullptr;
	std::vector<std::unique_ptr<Emitter>> emitter;
};