#pragma once
#include "pch.h"

//GEP:: Various low level objects required by the various rendering systems
struct RenderData
{
	//SDKMeshGO3D renderer
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::EffectFactory> m_fxFactory;

	Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;

	std::unique_ptr<DirectX::SpriteFont> m_font;
	int m_fontResNum;

	std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
	int m_resourceCount = 0;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	std::unique_ptr<DirectX::BasicEffect> m_effect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;

	std::unique_ptr<DirectX::BasicEffect> m_GPeffect;

	Camera* m_cam;

	~RenderData()
	{
		m_states.reset();
		m_fxFactory.reset();
		m_font.reset();
		m_resourceDescriptors.reset();
		m_spriteBatch.reset();
		m_effect.reset();
		m_batch.reset();
		m_GPeffect.reset();
	}
};