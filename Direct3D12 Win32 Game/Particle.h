#pragma once
#include "pch.h"
#include "ParticleFile.h"


class Particle
{
public:
	Particle() = default;
	~Particle() = default;

	void init(Vector2 pos, float _lifetime, float layer, bool fade, bool flip, Color colour, float scale);
	void init(Vector2 pos, float _lifetime, float layer, bool fade, bool flip, Color colour, float scale, Vector2 vel, Vector2 acc);
	void update(GameStateData* gsd);

	bool isDead() { return dead; };
	Vector2 getPos() { return offset_position + position; };
	bool getFlip() { return flip; };
	float getLayer() { return layer; };
	float getScale() { return scale; };
	Color getColour() { return colour; };

private:

	void SetVariables(DirectX::SimpleMath::Vector2 &pos, float _lifetime, float _layer, bool _fade, bool _flip, DirectX::SimpleMath::Color &_colour, float _scale);

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
	Color colour = Colors::White;
	bool increase_size = true;
	float scale = 1.f;
	bool dead = false;
};