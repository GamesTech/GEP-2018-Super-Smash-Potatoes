#pragma once
#include "pch.h"
#include "Particle.h"
#include "Emitter.h"

class ParticleSystem
{
public:
	ParticleSystem() = default;
	~ParticleSystem();
	bool init(RenderData* m_RD);
	void update(GameStateData* gsd);
	void render(RenderData* m_RD);

	void spawnParticle(int amount, Type::Type type, Vector2 pos, bool flipH);
	void spawnParticle(int amount, Type::Type type, Vector2 pos, bool flipH, Vector2 player_vel);

	void createEmitter(int amount, std::string image_name, Vector2 pos, bool flipH);

private:
	RenderData * m_RD = nullptr;
	std::vector<std::unique_ptr<Particle>> particles;
	std::vector<std::unique_ptr<Emitter>> emitter;
};