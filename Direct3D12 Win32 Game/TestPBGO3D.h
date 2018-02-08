#pragma once
#include "PBGO3D.h"
struct GameStateData;

class TestPBGO3D :
	public PBGO3D
{
public:
	TestPBGO3D();
	virtual ~TestPBGO3D();

	virtual void Init();
	virtual void Tick(GameStateData* _GSD);
};

