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

private:
	RenderData * m_RD = nullptr;
	std::vector<std::unique_ptr<Emitter>> emitter;
	std::vector<std::string> emitter_names;
	void loadEmitterFile(string _filename);
};