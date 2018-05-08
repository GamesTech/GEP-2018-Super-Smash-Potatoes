#include "pch.h"
#include "ImageBuffer.h"
#include <codecvt>
#include "RenderData.h"

ImageBuffer::TextureData ImageBuffer::loadTexture(RenderData* _RD, std::string image_name)
{
	//Check if the texture is already been created
	for (auto& texture : textures)
	{
		if (texture.m_image_name == image_name)
		{
			return texture; //If it has, returns the texture to the class that need it
		}
	}

	//If not, I will create a new texture and return that
	TextureData texture_data;
	texture_data.m_image_name = image_name;
	
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = "../DDS/" + texture_data.m_image_name + ".dds";
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	ResourceUploadBatch resourceUpload(_RD->m_d3dDevice.Get());

	resourceUpload.Begin();

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(_RD->m_d3dDevice.Get(), resourceUpload, wFilename.c_str(),
			texture_data.m_texture.ReleaseAndGetAddressOf()));


	CreateShaderResourceView(_RD->m_d3dDevice.Get(), texture_data.m_texture.Get(),
		_RD->m_resourceDescriptors->GetCpuHandle(texture_data.m_resourceNum = _RD->m_resourceCount++));

	auto uploadResourcesFinished = resourceUpload.End(_RD->m_commandQueue.Get());

	uploadResourcesFinished.wait();
	textures.push_back(texture_data); //Pushes the texture into the vector to been used again 
	return texture_data;
}
