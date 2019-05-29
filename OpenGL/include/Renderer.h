#pragma once

#include "TexturedModel.h" // Model.h
#include "Shader.h"

class Renderer
{
public:
	Renderer(Shader & shader);
	~Renderer();

	void Render(const Model & model);
	void Render(const TexturedModel & texturedModel);
private:
	Shader & m_Shader;
};

