#include "pch.h"
#include "Terrain.h"

unsigned int Terrain::Size = 800;
unsigned int Terrain::VertexCount = 128;
std::map<std::string, Model *> Terrain::TreesModels;
std::map<std::string, Model *> Terrain::PlantsModels;

Terrain::Terrain(const int & gridX, const int & gridZ, const unsigned int & textureID) :
	m_WorldPositionX(gridX * Size),
	m_WorldPositionZ(gridZ * Size),
	m_Mesh({}, {}, {}, {})
{
	m_Mesh = GenerateTerrain(textureID);
}

Terrain::Terrain(const Terrain & other) :
	m_WorldPositionX(other.m_WorldPositionX),
	m_WorldPositionZ(other.m_WorldPositionZ),
	m_Mesh(other.m_Mesh)
{
}

Terrain::Terrain(const Terrain && other) :
	m_WorldPositionX(other.m_WorldPositionX),
	m_WorldPositionZ(other.m_WorldPositionZ),
	m_Mesh(other.m_Mesh)
{
}

Terrain::~Terrain()
{
}

std::map<std::tuple<Shader*, Model*, unsigned int*>, std::vector<Entity>> Terrain::GenerateForest(Shader * shader)
{
	if (!shader)
	{
		std::cerr << "Failed to generate a forest, shader is not defined" << std::endl;
		return {};
	}
	if (TreesModels.size() == 0)
	{
		TreesModels = std::map<std::string, Model *>
		{
			std::make_pair("Oak", new Model("res/models/trees/Oak_Tree.obj")),
			std::make_pair("Poplar", new Model("res/models/trees/Poplar_Tree.obj")),
			std::make_pair("Fir", new Model("res/models/trees/Fir_Tree.obj")),
			std::make_pair("Palm", new Model("res/models/trees/Palm_Tree.obj")),
			std::make_pair("LowPoly", new Model("res/models/trees/Low_Poly.obj")),
		};
	}
	std::vector<Entity> oaks, poplar, fir, palm, low_poly;
	for (unsigned int i = 0; i < 150; ++i)
	{
		low_poly.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, - (rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 2.f));
		low_poly.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, - (rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 2.f));
		low_poly.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, - (rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 2.f));
		low_poly.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, - (rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 2.f));
		low_poly.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, - (rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 2.f));
	}

	std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>> forest
	{
		{ std::make_tuple(shader, TreesModels["LowPoly"], nullptr), low_poly }
	};
	return forest;
}

std::map<std::tuple<Shader*, Model*, unsigned int*>, std::vector<Entity>> Terrain::GenerateVegetation(Shader * shader)
{
	if (!shader)
	{
		std::cerr << "Failed to generate a forest, shader is not defined" << std::endl;
		return {};
	}
	if (PlantsModels.size() == 0)
	{
		PlantsModels = std::map<std::string, Model *>
		{
			std::make_pair("Fern", new Model("res/models/plants/Fern.obj", true)),
			std::make_pair("Grass", new Model("res/models/plants/Grass.obj", true, true)),
		};
	}
	std::vector<Entity> fern, grass;
	for (unsigned int i = 0; i < 150; ++i)
	{
		fern.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, -(rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 0.4f));
		grass.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, -(rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 1.f));
		fern.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, -(rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 0.4f));
		grass.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, -(rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 1.f));
		fern.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, -(rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 0.4f));
		grass.push_back(Entity({ (rand() % (int)Size - m_WorldPositionX), 0.f, -(rand() % (int)Size + m_WorldPositionZ) }, { 0, 0, 0 }, 1.f));
	}

	std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>> vegetation
	{
		{ std::make_tuple(shader, PlantsModels["Fern"], nullptr), fern },
		{ std::make_tuple(shader, PlantsModels["Grass"], nullptr), grass }
	};
	return vegetation;
}

Terrain & Terrain::operator=(Terrain & other)
{
	m_WorldPositionX = other.m_WorldPositionX;
	m_WorldPositionZ = other.m_WorldPositionZ;
	m_Mesh = other.m_Mesh;

	return *this;
}

Terrain & Terrain::operator=(Terrain && other)
{
	m_WorldPositionX = other.m_WorldPositionX;
	m_WorldPositionZ = other.m_WorldPositionZ;
	m_Mesh = other.m_Mesh;

	return *this;
}

Mesh Terrain::GenerateTerrain(const unsigned int & textureID)
{
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	// Vertices
	for (float i = 0.f; i < VertexCount; ++i)
	{
		for (float j = 0.f; j < VertexCount; ++j)
		{
			Vertex vertex;
			vertex.position = { -j / ((float)VertexCount - 1) * Size, 0.f, -i / ((float)VertexCount - 1) * Size };
			vertex.normal = { 0.f, 1.f, 0.f };
			vertex.texture_coordinate = { j / ((float)VertexCount - 1), i / ((float)VertexCount - 1) };
			vertex.tangent = { 0, 0, 0 };
			vertex.bitangent = { 0, 0, 0 };
			vertices.push_back(vertex);
		}
	}
	//Textures
	Texture texture;
	texture.id = textureID;
	texture.type = "texture_diffuse";
	texture.path = "";
	textures.push_back(texture);

	// Indices
	for (unsigned int gz = 0; gz < VertexCount - 1; ++gz)
	{
		for (unsigned int gx = 0; gx < VertexCount - 1; ++gx)
		{
			unsigned int top_left = (gz * VertexCount) + gx;
			unsigned int top_right = top_left + 1;
			unsigned int bottom_left = ((gz + 1) * VertexCount) + gx;
			unsigned int bottom_right = bottom_left + 1;
			indices.push_back(top_left);
			indices.push_back(bottom_left);
			indices.push_back(top_right);
			indices.push_back(top_right);
			indices.push_back(bottom_left);
			indices.push_back(bottom_right);
		}
	}

	return Mesh(vertices, textures, {}, indices);
}
