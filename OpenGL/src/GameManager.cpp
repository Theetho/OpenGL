#include <pch.h>
#include <GameManager.h>
#include <Renderer.h>	// Entity.h / TexturedModel.h / Shader.h / Model.h / Camera.h
#include <OBJLoader.h>
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
		 // positions			 // normals				 // texture coords
		-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f
	};

	Loader loader;
	Shader shaderCube("MasterShader");
	Shader shaderLamp("StaticShader");

	Renderer renderer(DisplayManager::GetWidth(), DisplayManager::GetHeight());

	Model cubeModel = loader.LoadToVao(vertices, {});
	TexturedModel untexturedModel(cubeModel, { 0 });
	TexturedModel container(cubeModel, { loader.LoadPNG("container2"),  loader.LoadPNG("container2_specular")});

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	Camera camera(glm::vec3(-3.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 1.f, 0.f), 0.05f, 0.1f);

	std::vector<DirectionalLight> dirLights
	{
		DirectionalLight({0, 0, 0}, {0.2f, 0.2f, 0.2f}, {0.5f, 0.5f, 0.5f}, {1.f, 1.f, 1.f})
	};
	std::vector<PointLight> pointsLights
	{
		PointLight({0.7f,  0.2f,  2.0f}, { 0.05f, 0.05f, 0.05f }, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}),
		PointLight({2.3f, -3.3f, -4.0f}, { 0.05f, 0.05f, 0.05f }, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}),
		PointLight({-4.0f,  2.0f, -12.0f}, { 0.05f, 0.05f, 0.05f }, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}),
		PointLight({0.0f,  0.0f, -3.0f}, { 0.05f, 0.05f, 0.05f }, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f})
	};
	std::vector<PointLight> pointLight
	{
		PointLight({0, 0, 0}, {0.4f, 0.4f, 0.4f}, {0.7f, 0.7f, 0.7f}, {1.f, 1.f, 1.f})
	};
	std::vector<SpotLight> spotLights
	{
		SpotLight(camera.GetPosition(), camera.GetTarget(), cos(12.5f), cos(17.5f), {1.f, 0.f, 0.f}, {1.f, 0.f, 0.f}, {1.f, 0.f, 0.f}),
	};
	// The cubes (containers
	std::vector<Entity> containers_entities;
	for (unsigned int i = 0; i < 10; ++i)
	{
		float angle = 20.f * i;
		containers_entities.push_back(Entity(container, cubePositions[i], { angle , 0.3f * angle, 0.5f * angle }, 1.f, Materials::Texture));
	}
	//The lamps
	std::vector<Entity> lamps_entities;
	for (unsigned int i = 0; i < pointsLights.size(); ++i)
		lamps_entities.push_back(Entity(untexturedModel, pointsLights[i].position, { 0, 0, 0 }, 0.2f, Materials::Light));
	for (unsigned int i = 0; i < spotLights.size(); ++i)
		lamps_entities.push_back(Entity(untexturedModel, spotLights[i].position, { 0, 0, 0 }, 0.2f, Materials::Light));

	// Maps
	std::map<const TexturedModel *, std::vector<Entity>> containers
	{
		std::make_pair(&containers_entities[0].GetTexturedModel(), containers_entities),
	};
	std::map<const TexturedModel *, std::vector<Entity>> lamps
	{
		std::make_pair(&lamps_entities[0].GetTexturedModel(), lamps_entities),
	};

	Entity lamp(untexturedModel, pointLight[0].position, { 0, 0, 0 }, 1.f, Materials::Light);

	shaderCube.Start();
//	shaderCube.SetVector3("camera_position", camera.GetPosition());
//	shaderCube.SetDirectionalLight(dirLights);
	shaderCube.SetPointLight(pointLight);
//	shaderCube.SetSpotLight(spotLights);
	shaderCube.SetTexturedMaterial();
	shaderCube.Stop();

	float time = 0.f, radius = 7.f;

	// Game loop
	while (DisplayManager::ShouldBeRunning())
	{
		DisplayManager::StartLoop();

		camera.Move();

		camera.LookAt();

		shaderCube.Start();
		shaderCube.SetVector3("camera_position", camera.GetPosition());
		pointLight[0].position = { sin(time) * radius, 0.f, cos(time) * radius };
		lamp.SetPosition(pointLight[0].position);
		shaderCube.SetPointLight(pointLight);
		shaderCube.Stop();

		renderer.Render(containers, camera, shaderCube);
		renderer.Render(lamp, camera, shaderLamp);
		/*for (auto & it : lamps)
		{
			unsigned int i = 0;
			for (i; i < pointsLights.size(); ++i)
			{
				shaderLamp.Start();
				shaderLamp.SetVector3("uniform_cube_color", pointsLights[i].specular);
				shaderLamp.Stop();
				renderer.Render(it.second[i], camera, shaderLamp);
			}
			for (i; i < pointsLights.size() + spotLights.size(); ++i)
			{
				shaderLamp.Start();
				shaderLamp.SetVector3("uniform_cube_color", spotLights[i - pointsLights.size()].specular);
				shaderLamp.Stop();
				renderer.Render(it.second[i], camera, shaderLamp);
			}
		}*/


		ToggleWireframeMode();

		DisplayManager::EndLoop();
		time += 0.01f;
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