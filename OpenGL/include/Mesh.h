#pragma once

#include "Shader.h"

////////////////////////////////////////////////////////////////////
/// From the tutorial on https://learnopengl.com/Model-Loading/Mesh
////////////////////////////////////////////////////////////////////
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coordinate;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
	bool hasTransparency;
	bool useFakeLight;
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex> & vertices, const std::vector<Texture> & textures, const std::vector<Material> & materials, const std::vector<unsigned int> & indices);
	Mesh(const Mesh & mesh)
	{
		m_Vertices	= mesh.m_Vertices;
		m_Textures	= mesh.m_Textures;
		m_Indices	= mesh.m_Indices;
		m_Materials = mesh.m_Materials;
		m_Vao		= mesh.m_Vao;
		m_Vbo		= mesh.m_Vbo;
		m_Ebo		= mesh.m_Ebo;
		m_Count		= mesh.m_Count;
	}
	Mesh(const Mesh && mesh)
	{
		m_Vertices	= mesh.m_Vertices;
		m_Textures	= mesh.m_Textures;
		m_Indices	= mesh.m_Indices;
		m_Materials = mesh.m_Materials;
		m_Vao		= mesh.m_Vao;
		m_Vbo		= mesh.m_Vbo;
		m_Ebo		= mesh.m_Ebo;
		m_Count		= mesh.m_Count;
	}
	~Mesh();

	Mesh & operator= (Mesh & mesh)
	{
		m_Vertices	= mesh.m_Vertices;
		m_Textures	= mesh.m_Textures;
		m_Indices	= mesh.m_Indices;
		m_Materials = mesh.m_Materials;
		m_Vao		= mesh.m_Vao;
		m_Vbo		= mesh.m_Vbo;
		m_Ebo		= mesh.m_Ebo;
		m_Count		= mesh.m_Count;

		return *this;
	}
	Mesh & operator= (Mesh && mesh)
	{
		m_Vertices	= mesh.m_Vertices;
		m_Textures	= mesh.m_Textures;
		m_Indices	= mesh.m_Indices;
		m_Materials = mesh.m_Materials;
		m_Vao		= mesh.m_Vao;
		m_Vbo		= mesh.m_Vbo;
		m_Ebo		= mesh.m_Ebo;
		m_Count		= mesh.m_Count;

		return *this;
	}

	std::vector<Vertex> m_Vertices;
	std::vector<Texture> m_Textures;
	std::vector<Material> m_Materials;
	std::vector<unsigned int> m_Indices;

	inline const unsigned int GetVao() const { return m_Vao; }
	inline const unsigned int GetVbo() const { return m_Vbo; }
	inline const unsigned int GetEbo() const { return m_Ebo; }
	inline const unsigned int GetCount() const { return m_Count; }

private:
	unsigned int m_Vao, m_Vbo, m_Ebo, m_Count;
	void SetupMesh();
};

