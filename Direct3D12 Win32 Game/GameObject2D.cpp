#include "pch.h"
#include "GameObject2D.h"


GameObject2D::GameObject2D()
{
}


GameObject2D::~GameObject2D()
{
}

void GameObject2D::SetRect(float left, float top, float right, float bottom)
{
	m_rect.left = left;
	m_rect.top = top;
	m_rect.right = right;
	m_rect.bottom = bottom;
	m_size.x = m_rect.right - m_rect.left;
	m_size.y = m_rect.bottom - m_rect.top;
}