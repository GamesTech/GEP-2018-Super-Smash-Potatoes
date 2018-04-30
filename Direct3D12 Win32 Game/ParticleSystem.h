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

	void spawnParticle(int amount, Type::Type type, Vector2 pos, bool flipH);
	void spawnParticle(int amount, Type::Type type, Vector2 pos, bool flipH, Vector2 player_vel);

private:
	RenderData * m_RD = nullptr;
	std::vector<std::unique_ptr<Particle>> particles;
};