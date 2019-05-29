#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(Shader & shader):
	m_Shader(shader)
{}

Renderer::~Renderer()
{}

void Renderer::Render(const Model & model)
{
	m_Shader.Start();
	glBindVertexArray(model.GetVaoID());
	glDrawElements(GL_TRIANGLES, model.GetCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	m_Shader.Stop();
}

void Renderer::Render(const TexturedModel & texturedModel)
{
	Model model = texturedModel.GetModel();
	m_Shader.Start();
	auto textures = texturedModel.GetTextures();
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
	glBindVertexArray(model.GetVaoID());
	glDrawElements(GL_TRIANGLES, model.GetCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	m_Shader.Stop();
}
