#pragma once
#include "GameObject2D.h"

class ImageBuffer
{
public:
	struct TextureData
	{
		std::string m_image_name = "";
		Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
		int m_resourceNum = -1;
	};

	TextureData loadTexture(RenderData* _RD, std::string image_name);

private:
	std::vector<TextureData> textures;
};