//
// Game.h
//

//GEP:: This project was created by braodly the following the docs for the DitrectXTK12 on their GitHub site.
// https://github.com/Microsoft/DirectXTK12/wiki 

#pragma once

#include "StepTimer.h"
#include "Audio.h"
#include "Text2D.h"
#include <vector>
#include "SceneManager.h"
#include "AudioManager.h"
#include "Input.h"

using std::vector;

struct RenderData;
struct GameStateData;

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game
{
public:

    Game();
    ~Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

//GEP:: These are the update and render fucntions for the main game class
//as called by the application system
    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void WaitForGpu() noexcept;
    void MoveToNextFrame();
    void GetAdapter(IDXGIAdapter1** ppAdapter);

    void OnDeviceLost();

    // Application state
    HWND                                                m_window;
	HWND												new_window;
    int                                                 m_outputWidth;
    int                                                 m_outputHeight;

    // Direct3D Objects
    D3D_FEATURE_LEVEL                                   m_featureLevel;
    static const UINT                                   c_swapBufferCount = 2;
    UINT                                                m_backBufferIndex;
    UINT                                                m_rtvDescriptorSize;
    Microsoft::WRL::ComPtr<ID3D12Device>                m_d3dDevice;
    Microsoft::WRL::ComPtr<IDXGIFactory4>               m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>          m_commandQueue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_rtvDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_dsvDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_commandAllocators[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;
    Microsoft::WRL::ComPtr<ID3D12Fence>                 m_fence;
    UINT64                                              m_fenceValues[c_swapBufferCount];
    Microsoft::WRL::Wrappers::Event                     m_fenceEvent;

    // Rendering resources
    Microsoft::WRL::ComPtr<IDXGISwapChain3>             m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargets[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_depthStencil;

    // Game state
    DX::StepTimer                                       m_timer;

	std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

	vector<GameObject2D*> m_2DObjects;

	RenderData* m_RD;
	Camera* m_cam;
	AudioManager* audio_manager;
	GameStateData* m_GSD;

	int menu_option_selected = 1;
	int resolution_option_selected = 1;
	bool settings_menu_open = false;
	int prevScene = 0;

	//audio system
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	std::unique_ptr<SceneManager> scene_manager;
	std::unique_ptr<Input> input_manager;

	//Debug
	Text2D * stateText;
};
