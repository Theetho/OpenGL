#pragma once

#include "Entity.h" // Model.h / TexturedModel.h
#include "Shader.h"
#include <Camera.h>

class Renderer
{
public:
	Renderer(const float & windowWidth, const float & windowHeight);
	~Renderer();

	void Render(const Model & model, Camera & camera, Shader & shader);
	void Render(const TexturedModel & texturedModel, Camera & camera, Shader & shader);
	void Render(std::map<const TexturedModel *, std::vector<Entity>> & entities, Camera & camera, Shader & shader);
	void Render(const Entity & entity, Camera & camera, Shader & shader);
private:
	glm::mat4 m_Model;
};

