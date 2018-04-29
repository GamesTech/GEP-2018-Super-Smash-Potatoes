#pragma once
#include "pch.h"
#include "ParticleFile.h"

class Particle
{
public:
	Particle() = default;
	~Particle() = default;

	void init(RenderData* m_RD, std::string type, Vector2 pos, bool flipH);
	void update(GameStateData* gsd);
	void render(RenderData* m_RD);

	bool isDead() { return lifetime <= 0.0f; };

private:
	std::unique_ptr<ParticleFile> particle_file = nullptr;
	std::unique_ptr<ImageGO2D> sprite = nullptr;

	Vector2 position = {0, 0};
	Vector2 velocity = { 0, 0 };
	Vector2 accelaration = { 0, 0 };

	float lifetime = 0;
};