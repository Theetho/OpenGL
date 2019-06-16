#include "pch.h"
#include "Renderer.h"

static bool display = true;

Renderer::Renderer(const float & windowWidth, const float & windowHeight):
	m_Model(1.f)
{}

Renderer::~Renderer()
{}

void Renderer::RenderEntities(std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>>& entities, Camera & camera)
{
	// For each pair of shader, model
	for (auto & pair : entities)
	{
		Shader * shader = std::get<0>(pair.first);
		Model * model = std::get<1>(pair.first);
		unsigned int * texture_id = std::get<2>(pair.first);
		shader->Start();
		shader->SetVector3("camera_position", camera.GetPosition());
		shader->SetMatrix("uniform_view", camera.GetView());
		shader->SetMatrix("uniform_projection", DisplayManager::GetProjection());
		// For each mesh of the model
		for (const Mesh & mesh : model->GetMesh())
		{
			// We use this order to bind a vao only once for each entity, but we have to set the model matrix a lot of time
			const Texture * texture = nullptr;
			// We bind the Vao of the mesh
			glBindVertexArray(mesh.GetVao());
			// For each entity we want to draw with this shader/model
			for (Entity & entity : pair.second)
			{
				// We bind the texture(s) of the mesh if it has some
				if (mesh.m_Textures.size())
				{
					shader->SetBool("material.texture", true);
					unsigned int diffuse_count = 0, specular_count = 0, normal_count = 0, height_count = 0;
					for (unsigned int i = 0; i < mesh.m_Textures.size(); ++i)
					{
						glActiveTexture(GL_TEXTURE0 + i);
						texture = &mesh.m_Textures[i];
						if (texture->hasTransparency)
							glDisable(GL_CULL_FACE);
						else
						{
							glEnable(GL_CULL_FACE);
							glCullFace(GL_BACK);
						}
						std::string number;
						std::string name = texture->type;
						if (name == "texture_diffuse")
							number = std::to_string(diffuse_count++);
						else if (name == "texture_specular")
							number = std::to_string(specular_count++);
						else if (name == "texture_normal")
							number = std::to_string(normal_count++);
						else if (name == "texture_height")
							number = std::to_string(height_count++);
						shader->SetInt("material." + name + number, i);
						shader->SetBool("uniform_fake_lighting", texture->useFakeLight);
						glBindTexture(GL_TEXTURE_2D, texture->id);
					}
				}
				// Else we bind the materials
				else if (mesh.m_Materials.size())
				{
					shader->SetBool("material.texture", false);
					for (unsigned int i = 0; i < mesh.m_Materials.size(); ++i)
					{	
						shader->SetVector3("material.ambient" + std::to_string(i), mesh.m_Materials[i].ambient);
						shader->SetVector3("material.diffuse" + std::to_string(i), mesh.m_Materials[i].diffuse);
						shader->SetVector3("material.specular" + std::to_string(i), mesh.m_Materials[i].specular);
					}
				}
				// We place the entity in the world
				m_Model = glm::mat4(1.f);
				m_Model = glm::translate(m_Model, entity.GetPosition());
				m_Model = glm::rotate(m_Model, entity.GetRotationX(), glm::vec3(1, 0, 0));
				m_Model = glm::rotate(m_Model, entity.GetRotationY(), glm::vec3(0, 1, 0));
				m_Model = glm::rotate(m_Model, entity.GetRotationZ(), glm::vec3(0, 0, 1));
				m_Model = glm::scale(m_Model, entity.GetScaleVector());
				shader->SetMatrix("uniform_model", m_Model);
				// And we draw it
				//glm::vec3 distance = camera.GetPosition() - entity.GetPosition();
				//if (abs(distance.x) < 200 || abs(distance.x) < 200)
					glDrawElements(GL_TRIANGLES, mesh.GetCount(), GL_UNSIGNED_INT, 0);
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		shader->Stop();
	}
}

void Renderer::RenderTerrain(std::vector<Terrain> & terrains, Camera & camera, Shader & shader)
{
	shader.Start();
	shader.SetVector3("camera_position", camera.GetPosition());
	shader.SetMatrix("uniform_view", camera.GetView());
	shader.SetMatrix("uniform_projection", DisplayManager::GetProjection());
	for (Terrain & terrain : terrains)
	{
		const Texture * texture = nullptr;
		const Mesh & mesh = terrain.GetMesh();
		glBindVertexArray(mesh.GetVao());
		unsigned int diffuse_count = 0, specular_count = 0, normal_count = 0, height_count = 0;
		for (unsigned int i = 0; i < mesh.m_Textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			texture = &mesh.m_Textures[i];
			std::string number;
			std::string name = texture->type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuse_count++);
			else if (name == "texture_specular")
				number = std::to_string(specular_count++);
			else if (name == "texture_normal")
				number = std::to_string(normal_count++);
			else if (name == "texture_height")
				number = std::to_string(height_count++);
			shader.SetInt("material." + name + number, i);
			glBindTexture(GL_TEXTURE_2D, texture->id);
			
		}
		m_Model = glm::mat4(1.f);
		m_Model = glm::translate(m_Model, { terrain.GetGridX(), 0.f, terrain.GetGridZ() });
		shader.SetMatrix("uniform_model", m_Model);
		glDrawElements(GL_TRIANGLES, mesh.GetCount(), GL_UNSIGNED_INT, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
