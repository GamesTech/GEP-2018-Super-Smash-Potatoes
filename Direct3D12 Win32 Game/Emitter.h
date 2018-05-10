#pragma once
#include "pch.h"
#include "Particle.h"

class Emitter :
	public ImageGO2D
{
public:
	Emitter(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer, std::string particle);
	~Emitter() = default;

	void update(GameStateData* gsd);
	void render(RenderData* m_RD);

	void addBurstOfParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, float x_range, float y_range);
	void addShootSideParticles(int amount, Vector2 pos, float lifetime, float layer, bool fade, bool flipH, Color colour, float scale, Vector2 vel, Vector2 acc);


private:
	std::vector<std::unique_ptr<Particle>> particles;
	std::string particle_file_name;
};