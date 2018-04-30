#pragma once
#include "pch.h"
#include "ParticleFile.h"

namespace Type
{
	enum Type
	{
		HIT,
		UPWARDS_PUNCH,
		ATTACK
	};
}

class Particle
{
public:
	Particle() = default;
	~Particle() = default;

	void init(RenderData* m_RD, Type::Type type, Vector2 pos, bool flipH);
	void update(GameStateData* gsd);
	void render(RenderData* m_RD);

	bool isDead() { return lifetime <= 0.0f; };

private:
	std::unique_ptr<ParticleFile> particle_file = nullptr;
	std::unique_ptr<ImageGO2D> sprite = nullptr;
	std::string file_name;

	Vector2 position = {0, 0};
	Vector2 velocity = { 0, 0 };
	Vector2 accelaration = { 0, 0 };

	float lifetime = 0;
};