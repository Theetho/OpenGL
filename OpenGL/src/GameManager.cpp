#include <pch.h>
#include <GameManager.h>
#include <Renderer.h>	// Entity.h / TexturedModel.h / Shader.h / Model.h / Camera.h
#include <OBJLoader.h>
#include <Light.h>

GameManager::GameManager() :
	m_WireframeMode(false),
	m_LightMode(Normal_Light),
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
	srand(time(NULL));

	Loader loader;
	Renderer renderer(DisplayManager::GetWidth(), DisplayManager::GetHeight());
	
	Shader masterShader("MasterShader");
	Shader noLightShader("NoLightShader");
	Shader fullLightShader("FullLightShader");
	Shader terrainShader("TerrainShader");

	Camera camera(glm::vec3(0.f, 2.f, -21.f), glm::vec3(0.f, 2.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 0.1f, 0.1f);

	/*Model nanosuit("res/models/nanosuit/nanosuit.obj");
	Model varyasuit("res/models/varyasuit/DolBarriersuit.obj");
	Model goblin("res/models/gobelin/goblin_pose_01.obj");
	unsigned int goblin_texture = loader.LoadPNG("res/models/gobelin/Goblin.png", true);
	Model stall("res/models/stall/stall.obj");
	unsigned int stall_texture = loader.LoadPNG("res/models/stall/stall.png");
	Model dragon("res/models/dragon/dragon.obj");
	unsigned int dragon_texture = loader.LoadPNG("res/textures/white.png");*/

	std::vector<DirectionalLight> dirLights
	{
		DirectionalLight({ 1, 0, 0 }, {0.2, 0.2, 0.2}, {0.5, 0.5, 0.5}, {1.0, 1.0, 1.0})
	};
	std::vector<PointLight> pointLights
	{
		PointLight({ 2, -7, -19 }, {0.05, 0.05, 0.05}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}),
		PointLight({ -3, 1.4, -21.4 }, {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, {1.0, 1.0, 1.0}),
		PointLight({ 4.2, -1.2, -23.3 }, {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, {1.0, 1.0, 1.0}),
		PointLight({ 2.1, 5.3, -22.9}, {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, {1.0, 1.0, 1.0})
	};
	std::vector<SpotLight> spotLights
	{
		SpotLight(camera.GetPosition(), camera.GetFront(), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), {0.3, 0.3, 0.3}, {0.6, 0.6, 0.6}, {0.9, 0.9, 0.9})
	};
	/*std::vector<Entity> nanosuit_entities
	{
		Entity({0.f, 0.f, -24.f}, {0.f, 0.f, 0.f}, 0.2f)
	};
	std::vector<Entity> varyasuit_entities
	{
		Entity({-4.f, 0.f, -24.f}, {0.f, 0.f, 0.f}, 0.2f)
	};
	std::vector<Entity> goblin_entities
	{
		Entity({-8.f, 0.5f, -24.f}, {-1.f, 0.f, 0.f}, 0.05f)
	};
	std::vector<Entity> stall_entities
	{
		Entity({-14.f, 0.f, -24.f}, {0.f, 2.f, 0.f}, 0.5f)
	};
	std::vector<Entity> dragon_entities
	{
		Entity({0.f, 0.f, -20.f}, {0.f, 2.f, 0.f}, 0.2f)
	};
	std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>> light_entities
	{
		{ std::make_tuple(&masterShader, &varyasuit, nullptr), varyasuit_entities},
		{ std::make_tuple(&masterShader, &nanosuit, nullptr), nanosuit_entities},
		{ std::make_tuple(&masterShader, &goblin, &goblin_texture), goblin_entities},
		{ std::make_tuple(&masterShader, &stall, &stall_texture), stall_entities},
		{ std::make_tuple(&masterShader, &dragon, &dragon_texture), dragon_entities},
	};
	std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>> no_light_entities
	{
		{ std::make_tuple(&noLightShader, &varyasuit, nullptr), varyasuit_entities},
		{ std::make_tuple(&noLightShader, &nanosuit, nullptr), nanosuit_entities},
		{ std::make_tuple(&noLightShader, &goblin, &goblin_texture), goblin_entities},
		{ std::make_tuple(&noLightShader, &stall, &stall_texture), stall_entities},
		{ std::make_tuple(&noLightShader, &dragon, &dragon_texture), dragon_entities},
	};
	std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>> full_light_entities
	{
		{ std::make_tuple(&fullLightShader, &varyasuit, nullptr), varyasuit_entities},
		{ std::make_tuple(&fullLightShader, &nanosuit, nullptr), nanosuit_entities},
		{ std::make_tuple(&fullLightShader, &goblin, &goblin_texture), goblin_entities},
		{ std::make_tuple(&fullLightShader, &stall, &stall_texture), stall_entities},
		{ std::make_tuple(&fullLightShader, &dragon, &dragon_texture), dragon_entities},
	};*/
	

	unsigned int terrainGrass = loader.LoadJPG("res/textures/vegetal/veg000.png");
	std::vector<Terrain> terrains
	{
		Terrain(0, 0, terrainGrass),
		Terrain(1, 0, terrainGrass)
	};

	auto forest = terrains[0].GenerateForest(&masterShader);
	for (unsigned int i = 1; i < terrains.size(); ++i)
	{
		auto trees = terrains[i].GenerateForest(&masterShader);
		for (auto & it : trees)
		{
			auto * tmp = &forest[it.first];
			tmp->insert(tmp->end(), it.second.begin(), it.second.end());
		}
	}
	auto vegetation = terrains[0].GenerateVegetation(&masterShader);
	for (unsigned int i = 1; i < terrains.size(); ++i)
	{
		auto plants = terrains[i].GenerateVegetation(&masterShader);
		for (auto & it : plants)
		{
			auto * tmp = &vegetation[it.first];
			tmp->insert(tmp->end(), it.second.begin(), it.second.end());
		}
	}

	masterShader.Start();
	masterShader.SetDirectionalLight(dirLights);
	masterShader.SetPointLight(pointLights);
	masterShader.SetFloat("material.shininess", 64.f);
	masterShader.Stop();
	
	terrainShader.Start();
	terrainShader.SetFloat("material.shininess", 64.f);
	terrainShader.Stop();

	// Game loop
	while (DisplayManager::ShouldBeRunning())
	{
		DisplayManager::StartLoop();

		camera.Move();

		camera.LookAt();

		/*switch (m_LightMode)
		{
			case GameManager::No_Light:		renderer.RenderEntities(no_light_entities, camera);		break;
			case GameManager::Normal_Light:	renderer.RenderEntities(light_entities, camera);		break;
			case GameManager::Full_Light:	renderer.RenderEntities(full_light_entities, camera);	break;
			default:																				break;
		}*/
		renderer.RenderEntities(forest, camera);
		renderer.RenderEntities(vegetation, camera);
		renderer.RenderTerrain(terrains, camera, terrainShader);


		ToggleModes();

		DisplayManager::EndLoop();
	}
}

void GameManager::ToggleModes()
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
	if (GetKeyOnce(SDL_SCANCODE_E))
	{
		switch (m_LightMode)
		{
			case No_Light:		m_LightMode = Normal_Light;		break;
			case Normal_Light:	m_LightMode = Full_Light;		break;
			case Full_Light:	m_LightMode = No_Light;			break;
			default:											break;
		}
	}

}

bool GameManager::Initialize()
{
	DisplayManager::Create();
	InputManager::Create();

	return true;
}