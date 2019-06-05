#pragma once

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Run();

private:
	enum LightMode { No_Light, Normal_Light, Full_Light };

	double m_DeltaTime;
	bool m_WireframeMode;
	LightMode m_LightMode;

	bool Initialize();
	void ToggleModes();
};

