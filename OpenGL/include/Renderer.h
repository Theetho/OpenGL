#pragma once

#include "Entity.h" // Model.h / TexturedModel.h / Mesh.h
#include "Shader.h"
#include "Camera.h"
#include "Terrain.h"

class Renderer
{
public:
	Renderer(const float & windowWidth, const float & windowHeight);
	~Renderer();

	// This function allows us to draw multiple entities associated to a Model and a Shader
	void RenderEntities(std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>>& entities, Camera & camera);
	void RenderTerrain(std::vector<Terrain> & terrains, Camera & camera, Shader & shader);
private:
	glm::mat4 m_Model;
};

