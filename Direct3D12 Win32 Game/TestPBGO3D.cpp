#include "pch.h"
#include "TestPBGO3D.h"
#include "GameStateData.h"


TestPBGO3D::TestPBGO3D()
{
}


TestPBGO3D::~TestPBGO3D()
{
}

void TestPBGO3D::Tick(GameStateData* _GSD)
{
	static float time = 0.0f;
	time += _GSD->m_dt;
	m_pos.x = 12.0f * cos(time);
	m_pos.z = 12.0f * sin(time);

	m_yaw += _GSD->m_dt;

	PBGO3D::Tick(_GSD);
}

void TestPBGO3D::Init()
{
	m_vertices = new VertexPositionColor[3];
	m_numTris = 1;
	m_vertices[0] = VertexPositionColor(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	m_vertices[1] = VertexPositionColor(Vector3(0.5f, -0.5f, 0.5f), Colors::Red);
	m_vertices[2] = VertexPositionColor(Vector3(-0.5f, -0.5f, 0.5f), Colors::Blue);
}
