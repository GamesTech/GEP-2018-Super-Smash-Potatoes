#pragma once
#include "GameObject2D.h"
#include <string>
#include "ImageBuffer.h"

using std::string;
struct RenderData;

//GEP:: Builds on the Base Game Object 2D class to allow images to be displayed to screen.
// These need to made into dds format objects which can be done using the Images project in the Asset Pipeline solution folder.
// Based on this part of the DirectXTK12 wiki documentation
//https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures 

class ImageGO2D :
	public GameObject2D
{
public:
	ImageGO2D(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer);
	virtual ~ImageGO2D();

	void Render(RenderData* _RD);

	void SetBoundingBoxes();
	void SetBoundingBoxes(float left, float top, float right, float bottom);
	void CentreOrigin();
	void BottomOrigin();
	void SetOrientation(float new_orientation);
	void FlipH(bool flip);
	bool GetFlipH();
	void SetOpacity(float amount);
	
	

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
	int m_resourceNum = -1;
	SpriteEffects m_flip = SpriteEffects_None;
	ImageBuffer::TextureData m_texture_data;
};

