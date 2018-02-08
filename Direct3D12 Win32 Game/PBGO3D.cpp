#include "pch.h"
#include "PBGO3D.h"
#include "RenderData.h"


PBGO3D::PBGO3D()
{
	m_type = GO3D_RT_PRIM;
}


PBGO3D::~PBGO3D()
{
	if (m_vertices)
	{
		delete[] m_vertices;
	}
}

void PBGO3D::Render(RenderData * _RD)
{
	//TODO: Add normals so can use EffectFlags::Lighting like pre-built geometry
	//TODO: Lights and other bits of defualt lighting as objects to play with
	_RD->m_effect->SetWorld(m_world);
	int vert = 0;
	for (int i = 0; i < m_numTris; i++)
	{
		_RD->m_batch->DrawTriangle(m_vertices[vert++], m_vertices[vert++], m_vertices[vert++]);
	}
}
