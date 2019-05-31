#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(const float & windowWidth, const float & windowHeight):
	m_Model(1.f)
{}

Renderer::~Renderer()
{}

void Renderer::Render(const Model & model, Camera & camera, Shader & shader)
{
	shader.Start();
	shader.SetMatrix("uniform_view", camera.GetView());
	shader.SetMatrix("uniform_projection", DisplayManager::GetProjection());
	glBindVertexArray(model.GetVaoID());
	//glDrawElements(GL_TRIANGLES, model.GetCount(), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, model.GetCount());
	glBindVertexArray(0);
	shader.Stop();
}

void Renderer::Render(const TexturedModel & texturedModel, Camera & camera, Shader & shader)
{
	const Model & model = texturedModel.GetModel();
	shader.Start();
	shader.SetMatrix("uniform_view", camera.GetView());
	shader.SetMatrix("uniform_projection", DisplayManager::GetProjection());
	auto & textures = texturedModel.GetTextures();
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
	glBindVertexArray(model.GetVaoID());
	//glDrawElements(GL_TRIANGLES, model.GetCount(), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, model.GetCount());
	glBindVertexArray(0);
	shader.Stop();
}

void Renderer::Render(std::map<TexturedModel *, std::vector<Entity>> & entities, Camera & camera, Shader & shader)
{
	shader.Start();
	shader.SetMatrix("uniform_view", camera.GetView());
	shader.SetMatrix("uniform_projection", DisplayManager::GetProjection());
	for (auto & pair : entities)
	{
		Model model = pair.first->GetModel();
		auto & textures = pair.first->GetTextures();
		shader.SetSamplers(textures.size());
		for (unsigned int i = 0; i < textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}
		glBindVertexArray(model.GetVaoID());
		for (auto & entity : pair.second)
		{
			m_Model = glm::mat4(1.f);
			m_Model = glm::translate(m_Model, entity.GetPosition());
			m_Model = glm::rotate(m_Model, entity.GetRotationX(), glm::vec3(1, 0, 0));
			m_Model = glm::rotate(m_Model, entity.GetRotationY(), glm::vec3(0, 1, 0));
			m_Model = glm::rotate(m_Model, entity.GetRotationZ(), glm::vec3(0, 0, 1));
			m_Model = glm::scale(m_Model, entity.GetScaleVector());
			shader.SetMatrix("uniform_model", m_Model);
			//glDrawElements(GL_TRIANGLES, model.GetCount(), GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, model.GetCount());
		}
		glBindVertexArray(0);
	}
	shader.Stop();
}

void Renderer::Render(const Entity & entity, Camera & camera, Shader & shader)
{
	shader.Start();
	shader.SetMatrix("uniform_view", camera.GetView());
	shader.SetMatrix("uniform_projection", DisplayManager::GetProjection());
	Model model = entity.GetTexturedModel().GetModel();
	auto & textures = entity.GetTexturedModel().GetTextures();
	shader.SetSamplers(textures.size());
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
	glBindVertexArray(model.GetVaoID());
	m_Model = glm::mat4(1.f);
	m_Model = glm::translate(m_Model, entity.GetPosition());
	m_Model = glm::rotate(m_Model, entity.GetRotationX(), glm::vec3(1, 0, 0));
	m_Model = glm::rotate(m_Model, entity.GetRotationY(), glm::vec3(0, 1, 0));
	m_Model = glm::rotate(m_Model, entity.GetRotationZ(), glm::vec3(0, 0, 1));
	m_Model = glm::scale(m_Model, entity.GetScaleVector());
	shader.SetMatrix("uniform_model",	m_Model);
	//glDrawElements(GL_TRIANGLES, model.GetCount(), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, model.GetCount());
	glBindVertexArray(0);
	shader.Stop();
}
