#pragma once
#include "pch.h"
#include "Particle.h"

class ParticleSystem
{
public:
	ParticleSystem() = default;
	~ParticleSystem() = default;
	bool init(RenderData* m_RD);
	void update(GameStateData* gsd);
	void render(RenderData* m_RD);

	void spawnParticle(int amount, std::string type, Vector2 pos, bool flipH);

private:
	RenderData * m_RD = nullptr;
	std::vector<std::unique_ptr<Particle>> particles;
};