#pragma once
#include "GameObject3D.h"

//GEP:: Allows an array of vertices to be described in code and then creates a 3D model from it
// NOTE this is note a very efficient way of doing this as it effectively recreates the model EVERY frame it is rendered.
// https://github.com/Microsoft/DirectXTK12/wiki/Simple-rendering 

class PBGO3D :
	public GameObject3D
{
public:
	PBGO3D();
	virtual ~PBGO3D();

	virtual void Init()=0;
	virtual void Render(RenderData* _RD);

protected:
	int m_numTris = 0;
	VertexPositionColor* m_vertices = NULL;
};

