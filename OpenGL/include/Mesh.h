#pragma once

////////////////////////////////////////////////////////////////////
/// From the tutorial on https://learnopengl.com/Model-Loading/Mesh
////////////////////////////////////////////////////////////////////
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texture_coordinate;
};

struct Texture
{
	unsigned int id;
	std::string type;
};


class Mesh
{
public:
	Mesh(const std::vector<Vertex> & vertices, const std::vector<Texture> & textures, const std::vector<unsigned int> & indices);
	~Mesh();

	std::vector<Vertex> m_Vertices;
	std::vector<Texture> m_Textures;
	std::vector<unsigned int> m_Indices;
private:
	unsigned int m_Vao, m_Vbo, m_Ebo;
	void SetupMesh();
};

