#pragma once

class Input
{
public:
	Input();
	~Input();

	void init();
	void update(GameStateData* gsd);

	void ResumeInput();
	void SuspendInput();

private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::GamePad> m_gamePad;

	DirectX::GamePad::ButtonStateTracker m_buttons;


};