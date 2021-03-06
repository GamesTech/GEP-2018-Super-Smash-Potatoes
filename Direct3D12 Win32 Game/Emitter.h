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

	void addParticles(int amount, Vector2 pos, bool flipH);

private:
	std::vector<std::unique_ptr<Particle>> particles;
	std::shared_ptr<ParticleFile> particle_file;
};