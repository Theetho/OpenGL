#pragma once

class InputManager;

////////////////////////////////////////////////////////////
/// \brief Static class that manages the window display 
///
////////////////////////////////////////////////////////////
class DisplayManager
{
public:
	static void Create(float w = 800.f, float h = 600.f);
	static void StartLoop();
	static void EndLoop();
	static void Destroy();
	static void Zoom(const bool & in);
	inline static bool ShouldBeRunning() { return DisplayManager::IsRunning; }

	inline static const float & GetWidth() { return Width; }
	inline static const float & GetHeight() { return Height; }
	inline static const glm::vec4 & GetClearColor() { return ClearColor; }
	inline static const glm::mat4 & GetProjection() { return Projection; }
	inline static SDL_Window * GetWindow() { return Window; }

private:
	static SDL_Window * Window;
	static SDL_GLContext Settings;
	static bool IsInstantiated, IsRunning;
	static float Width, Height;
	static float FpsCap;
	static double Fovy;
	static Uint32 StartTime, DeltaTime;
	static glm::vec4 ClearColor;
	static glm::mat4 Projection;

	DisplayManager() = delete;
	~DisplayManager() = delete;

	friend InputManager;
};

