#include "pch.h"
#include "Animation2D.h"

Animation2D::Animation2D(RenderData* _RD, string _filename) :ImageGO2D(_RD, _filename)
{

}

Animation2D::~Animation2D()
{

}

void Animation2D::AnimationOn()
{
	m_animation_on = true;
}
