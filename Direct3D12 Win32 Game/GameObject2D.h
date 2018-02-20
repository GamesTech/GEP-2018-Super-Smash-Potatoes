#pragma once
#include "pch.h"

//GEP:: Base class for all 2-D objects

using namespace DirectX;
using namespace DirectX::SimpleMath;
struct RenderData;
struct GameStateData;

class GameObject2D
{
public:
	GameObject2D();
	virtual ~GameObject2D();

	Vector2 GetPos() { return m_pos; }
	float Left () { return m_min.x; }
	float Right () { return m_max.x; }
	float Top() { return m_min.y; }
	float Bottom() { return m_max.y; }
	float Width() { return m_size.x; }
	float Height() { return m_size.y; }

	Vector2 GetOrigin() { return m_origin; }
	float GetOri() { return m_orientation; }
	Color GetColour() { return m_colour; }
	Vector2 GetScale() { return m_scale; }
	float GetLayer() { return m_layer;  }
	std::string GetTag() { return tag; }
	void SetPos(Vector2 _pos) { m_pos = _pos; }
	void SetOrigin(Vector2 _origin) { m_origin = _origin; }
	void SetOri(float _ori) { m_orientation = _ori; }
	void SetColour(Color _col) { m_colour = _col; }
	void SetScale(Vector2 _scale) { m_scale = _scale; }
	void SetLayer(float _layer) { m_layer = _layer;  }


	virtual void CentreOrigin() = 0;
	virtual void BottomOrigin() = 0;

	virtual void Tick(GameStateData* _GSD) {};
	virtual void Render(RenderData* _RD) = 0;

protected:
	Vector2 m_pos = Vector2::Zero;
	Vector2 m_origin = Vector2::Zero;
	Vector2 m_centre = Vector2::Zero;
	float m_orientation = 0.0f;
	float m_layer = 0.0f;
	Color m_colour = Colors::White;
	Vector2 m_scale = Vector2::One;
	Vector2 m_min = Vector2::Zero;
	Vector2 m_max = Vector2::Zero;
	Vector2 m_size = Vector2::Zero;

	std::string tag;
	//bool m_grounded = false;

};

