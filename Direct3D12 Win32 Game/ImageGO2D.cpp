#include "pch.h"
#include "ImageGO2D.h"
#include <codecvt>
#include "RenderData.h"




ImageGO2D::ImageGO2D(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer)
{
	image_name = _filename;
	m_texture_data = image_buffer->loadTexture(_RD, _filename);
}


ImageGO2D::~ImageGO2D()
{
	m_texture_data.m_texture.Reset();
}

void ImageGO2D::Render(RenderData* _RD)
{
	_RD->m_spriteBatch->Draw(_RD->m_resourceDescriptors->GetGpuHandle(m_texture_data.m_resourceNum),
		GetTextureSize(m_texture_data.m_texture.Get()),
		m_pos, &m_rect, m_colour, m_orientation, m_origin, m_scale, m_flip, m_layer);
	//TODO::add sprite effects & layer Depth
	//TODO::example stuff for sprite sheet
}

void ImageGO2D::SetBoundingBoxes()
{
	XMUINT2 size = GetTextureSize(m_texture_data.m_texture.Get());

	m_min.x = m_pos.x;
	m_min.y = m_pos.y;
	m_max.x = m_pos.x + m_size.x;
	m_max.y = m_pos.y + m_size.y;

}

void ImageGO2D::SetBoundingBoxes(float left, float top, float right, float bottom)
{

	m_min.x = left;
	m_min.y = top;
	m_max.x = right;
	m_max.y = bottom;

}

void ImageGO2D::CentreOrigin()
{
	m_origin.x = float(m_size.x / 2);
	m_origin.y = float(m_size.y / 2);
}

void ImageGO2D::BottomOrigin()
{
	XMUINT2 size = GetTextureSize(m_texture_data.m_texture.Get());

	//m_min.x = m_origin.x;
	//m_min.y = m_origin.y;
	//m_max.x = m_origin.x + size.x;
	//m_max.y = m_origin.y + size.y;
	
	m_origin.x = float(m_size.x / 2);
	m_origin.y = float(m_size.y);
}

void ImageGO2D::SetOrientation(float new_orientation)
{
	m_orientation = m_orientation + new_orientation;
}

void ImageGO2D::FlipH(bool flip)
{
	if (flip)
	{
		m_flip = SpriteEffects_FlipHorizontally;
	}
	else
	{
		m_flip = SpriteEffects_None;
	}
}

bool ImageGO2D::GetFlipH()
{
	if (m_flip == SpriteEffects_FlipHorizontally)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ImageGO2D::SetOpacity(float amount)
{
	m_colour.w = amount;
}
