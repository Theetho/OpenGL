#pragma once

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Run();

private:
	double m_DeltaTime;
	bool m_WireframeMode;

	bool Initialize();
	void ToggleWireframeMode();
};

