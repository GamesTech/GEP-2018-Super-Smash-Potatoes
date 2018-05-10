#pragma once
#include "pch.h"
#include "ParticleFile.h"


class Particle
{
public:
	Particle() = default;
	~Particle() = default;
	void init(std::string particle_file, Vector2 pos, bool flip);
	void update(GameStateData* gsd);

	void SetVariables(Vector2 pos, bool _flip);

	void randomColours();

	bool isDead() { return dead; };
	Vector2 getPos() { return offset_position + position; };
	bool getFlip() { return flip; };
	float getLayer() { return layer; };
	float getScale() { return scale; };
	Color getColour() { return colour; };

private:
	void isFlip();
	void burstParticles();

	std::unique_ptr<ParticleFile> particle_file;

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
	bool random_colour = false;
	Color colour = Colors::White;
	bool increase_size = true;
	float scale = 1.f;
	bool dead = false;
};