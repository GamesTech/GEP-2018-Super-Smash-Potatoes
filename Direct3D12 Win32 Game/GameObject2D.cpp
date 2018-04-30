#include "pch.h"
#include "GameObject2D.h"


GameObject2D::GameObject2D()
{
}


GameObject2D::~GameObject2D()
{
}

float GameObject2D::Width()
{
	if (m_width == 0)
	{
		return m_size.x;
	}
	else
	{
		return m_width;
	}
}

float GameObject2D::Height()
{
	if (m_height == 0)
	{
		return m_size.y;
	}
	else
	{
		return m_height;
	}
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

void GameObject2D::SetRect(RECT _rect)
{
	m_rect = _rect;
	m_size.x = m_rect.right - m_rect.left;
	m_size.y = m_rect.bottom - m_rect.top;
}
