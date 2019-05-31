#include <pch.h>
#include <GameManager.h>
#include <Renderer.h>	// Entity.h / TexturedModel.h / Shader.h / Model.h / Camera.h
#include <Loader.h>
#include <Light.h>

GameManager::GameManager() :
	m_WireframeMode(false),
	m_DeltaTime(0.0)
{
	if (!Initialize())
	{
		throw std::exception("Failed to initialize the game");
	}
}

GameManager::~GameManager()
{
	DisplayManager::Destroy();
	InputManager::Destroy();
}

void GameManager::Run()
{
	std::vector<float> vertices
	{
		// Positions	        // Textures
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f
	};

	Loader loader;
	Shader shaderCube("StaticShader");
	Shader shaderLamp("StaticShader");

	Renderer renderer(DisplayManager::GetWidth(), DisplayManager::GetHeight());
	Light light({ 0, 3, 0 }, { 1, 1, 1, 1 }, 0.5f);

	Model cubeModel(loader.LoadToVao(vertices, {}), vertices.size() / 5);
	TexturedModel untexturedModel(cubeModel, { (unsigned int)0 });
	
	Entity cube(untexturedModel, { 0, 0, 0 }, { 0, 0, 0 }, 1.f);
	Entity lamp(untexturedModel, light.GetPosition(), { 0, 0, 0 }, 0.5f);
	
	Camera camera(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f), 0.05f, 0.05f);

	shaderCube.Start();
	shaderCube.SetVector4("uniform_cube_color", glm::vec4(1.f, 0.5f, 0.31f, 1.f));
	shaderCube.SetVector4("uniform_light_color", light.GetColor() * light.GetAmbientLighting());
	shaderCube.Stop();

	shaderLamp.Start();
	shaderLamp.SetVector4("uniform_cube_color", light.GetColor());
	shaderLamp.SetVector4("uniform_light_color", light.GetColor());
	shaderLamp.Stop();

	// Game loop
	while (DisplayManager::ShouldBeRunning())
	{
		DisplayManager::StartLoop();

		camera.Move();

		camera.LookAt();

		renderer.Render(cube, camera, shaderCube);
		renderer.Render(lamp, camera, shaderLamp);

		ToggleWireframeMode();

		DisplayManager::EndLoop();
	}
}

void GameManager::ToggleWireframeMode()
{
	if (GetKeyOnce(SDL_SCANCODE_Q))
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

}

bool GameManager::Initialize()
{
	DisplayManager::Create();
	InputManager::Create();

	return true;
}