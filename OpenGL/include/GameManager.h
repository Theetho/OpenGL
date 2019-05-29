#pragma once

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Run();
	void ToggleWireframeMode();

	// Function that returns true vhen the user presses the key, but only once
	inline bool glfwGetKeyPressed(GLFWwindow * window, const unsigned int & key)
	{
		return (glfwGetKey(window, key) == GLFW_PRESS ? (m_KeyPressed[key] ? false : (m_KeyPressed[key] = true)) : m_KeyPressed[key] = false);
	}
	// Function that returns true when the user releases the key
	inline bool glfwGetKeyReleased(GLFWwindow * window, const unsigned int & key)
	{
		return (glfwGetKey(window, key) == GLFW_PRESS ? (m_KeyPressed[key] ? false : (!(m_KeyPressed[key] = true))) : (m_KeyPressed[key] ? !(m_KeyPressed[key] = false) : false));
	}


private:
	GLFWwindow * m_Window; 
	float m_Width, m_Height;
	char m_KeyPressed[GLFW_KEY_LAST + 1];
	bool m_WireframeMode;

	bool Initialize();
};

