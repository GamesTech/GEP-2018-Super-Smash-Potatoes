#pragma once
#include "pch.h"
#include "Particle.h"

class Emitter :
	public ImageGO2D
{
public:
	Emitter(RenderData* _RD, string _filename);
	~Emitter() = default;
	void init(RenderData* m_RD);
	void update(GameStateData* gsd);
	void render(RenderData* m_RD);

	void addBurstOfParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale);
	void addShootSideParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, Vector2 vel, Vector2 acc);


private:

	struct Particles
	{
		float lifetime = 0.0f;
		float max_lifetime = 0.0f;
		Vector2 position = { 0, 0 };
		Vector2 offset_position = { 0, 0 };
		Vector2 velocity = { 0, 0 };
		Vector2 accelaration = { 0, 0 };
		bool fade = true;
		float opacity = 1;
		bool flip = false;
		float layer = 0.0f;
	};

	void normalization(Particles& temp_p);

	std::vector<std::unique_ptr<Particle>> particles;
};