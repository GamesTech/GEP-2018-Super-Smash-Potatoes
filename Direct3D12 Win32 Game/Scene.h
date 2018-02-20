#pragma once
class Scene
{
public:
	Scene() = default;
	virtual ~Scene();

	void virtual init(RenderData* m_RD) {};
	void virtual update(GameStateData* gsd) {};
	void virtual render(RenderData* m_RD,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList) {};
	void virtual ReadInput(GameStateData* gsd) {};

	GameObject2D& getObjectByIndex(int index) const;
	GameObject2D& getObjectByTag(std::string tag) const;

protected:
	std::vector<GameObject2D*> game_objects;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<DirectX::GamePad> m_gamePad;
};