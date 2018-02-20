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
#include "AudioManager.h"

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

	vector<GameObject3D*> m_3DObjects;
	vector<GameObject2D*> m_2DObjects;
	vector<GameObject2D*> m_2DPlatforms;

	//Player2D* m_testPlatform;
	//Player2D* m_testPlatform2;
	Player2D* m_player;
	RenderData* m_RD;
	Camera* m_cam;
	AudioManager* audio_manager;

	//Player2D* player_one;
	Text2D* title_text;
	ImageGO2D* start_game_button;
	ImageGO2D* settings_button;
	ImageGO2D* quit_button;
	Text2D* resolution_text;
	ImageGO2D* main_menu_button;



	GameStateData* m_GSD;

	//GEP:: Keyboard and Mouse Abstractions for basic input system
	void ReadInput();
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<DirectX::GamePad> m_gamePad;

	int menu_option_selected = 1;
	int resolution_option_selected = 1;
	bool settings_menu_open = false;
	//audio system
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	int menu_option_selected = 0;
	void highlight_option_selected();

	void loadMenu();
	void loadSettings();
	void loadGame();

	void newResolutionText(int new_resolution_option);

	//Debug
	Text2D * stateText;

	DirectX::GamePad::ButtonStateTracker m_buttons;
};
