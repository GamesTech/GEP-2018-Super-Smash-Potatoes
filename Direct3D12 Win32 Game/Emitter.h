#pragma once
#include "pch.h"
#include "ParticleFile.h"

class Emitter :
	public ImageGO2D
{
public:
	Emitter(RenderData* _RD, string _filename);
	~Emitter() = default;
	void init(RenderData* m_RD);
	void update(GameStateData* gsd);
	void render(RenderData* m_RD);

	void addParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH);

private:

	struct Particles
	{
		float lifetime = 0;
		Vector2 position = { 0, 0 };
		Vector2 offset_position = { 0, 0 };
		Vector2 velocity = { 0, 0 };
		Vector2 accelaration = { 0, 0 };
		bool fade = true;
		float opacity = 1;
		bool flip = false;
		float layer = 0.0f;
	};


	std::vector<Particles> particles;
};