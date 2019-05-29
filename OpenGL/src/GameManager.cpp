#include <pch.h>
#include <GameManager.h>
#include <Renderer.h>	// TexturedModel.h / Shader.h / Model.h
#include <Loader.h>



static void FrameBufferSizeCallback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void ProcessInput(GLFWwindow * window, GameManager * gameManager)
{
	if (gameManager->glfwGetKeyReleased(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	if (gameManager->glfwGetKeyPressed(window, GLFW_KEY_Q))
		gameManager->ToggleWireframeMode();
}

GameManager::GameManager() :
	m_WireframeMode(false),
	m_Width(800.f),
	m_Height(600.f)
{
	if (!Initialize())
	{
		throw std::exception("Failed to initialize the game");
	}
}

GameManager::~GameManager()
{
	glfwTerminate();
}

void GameManager::Run()
{
	std::vector<float> vertices
	{
			// Positions		// Colors		  // Textures
		-0.5f, -0.5f, 0.0f,		1.f, 0.f, 0.f,		0.f, 0.f,
		 0.5f, -0.5f, 0.0f,		0.f, 1.f, 0.f,		0.f, 1.f,
		 0.5f,  0.5f, 0.0f,		0.f, 0.f, 1.f,		1.f, 1.f,
		-0.5f,  0.5f, 0.0f,		1.f, 1.f, 1.f,		1.f, 0.f,
	};

	std::vector<unsigned int> indices
	{
		0, 1, 2,
		0, 3, 2
	};

	Loader loader;
	Shader shader("StaticShader");
	Renderer renderer(shader);
	Model square(loader.LoadToVao(vertices, indices), indices.size());
	TexturedModel texturedModel(square, { loader.LoadJPG("container"), loader.LoadPNG("awesomeface") });

	glm::mat4 model(1.f), view(1.f), projection(1.f);
	model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));
	view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));
	projection = glm::perspective(glm::radians(45.f), m_Width / m_Height, 0.1f, 100.f);
	
	shader.Start();
	shader.SetSamplers(texturedModel.GetTextures().size());
	shader.SetMatrix("uniform_model", model);
	shader.SetMatrix("uniform_view", view);
	shader.SetMatrix("uniform_projection", projection);
	shader.Stop();

	// Game loop
	while (!glfwWindowShouldClose(m_Window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		ProcessInput(m_Window, this);

		renderer.Render(texturedModel);

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void GameManager::ToggleWireframeMode()
{
	if (m_WireframeMode)
	{
		m_WireframeMode = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		m_WireframeMode = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

}

bool GameManager::Initialize()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	// Initialize OpenGL to version 3.3 core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Version_0_0_1", nullptr, nullptr);
	if (!m_Window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Create OpenGl contexte
	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glViewport(0, 0, m_Width, m_Height);
	glClearColor(0.7f, 0.2f, 0.2f, 1.0f);

	glfwSetFramebufferSizeCallback(m_Window, FrameBufferSizeCallback);

	return true;
}