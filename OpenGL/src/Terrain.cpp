#include "pch.h"
#include "Terrain.h"

unsigned int Terrain::Size = 200;
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
	glm::vec2 tree_pos(m_WorldPositionX + Size / 30, m_WorldPositionZ + Size / 30);
	while (tree_pos.x < m_WorldPositionX + Size)
	{
		tree_pos.y = m_WorldPositionZ + Size / 30;
		while (tree_pos.y < m_WorldPositionZ + Size)
		{
			int index = rand() % TreesModels.size();
			switch (index)
			{
			case 0:	oaks.push_back(Entity({ -tree_pos.x, 0.f, -tree_pos.y }, { 0, 0, 0 }, 1.f));		break;
			case 1:	fir.push_back(Entity({ -tree_pos.x, 0.f, -tree_pos.y }, { 0, 0, 0 }, 1.f));			break;
			case 2:	palm.push_back(Entity({ -tree_pos.x, 0.f, -tree_pos.y }, { 0, 0, 0 }, 1.f));		break;
			case 3:	poplar.push_back(Entity({ -tree_pos.x, 0.f, -tree_pos.y }, { 0, 0, 0 }, 1.f));		break;
			case 4:	low_poly.push_back(Entity({ -tree_pos.x, 0.f, -tree_pos.y }, { 0, 0, 0 }, 2.f));	break;
			default:	break;
			}
			tree_pos.y += Size / 30;
		}
		tree_pos.x += Size / 30;
	}

	std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>> forest
	{
		{ std::make_tuple(shader, TreesModels["Oak"], nullptr), oaks },
		{ std::make_tuple(shader, TreesModels["Poplar"], nullptr), poplar },
		{ std::make_tuple(shader, TreesModels["Fir"], nullptr), fir },
		{ std::make_tuple(shader, TreesModels["Palm"], nullptr), palm },
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
	glm::vec2 plant_pos(m_WorldPositionX + Size / 15, m_WorldPositionZ + Size / 15);
	while (plant_pos.x < m_WorldPositionX + Size - Size / 15)
	{
		plant_pos.y = m_WorldPositionZ + Size / 15;
		while (plant_pos.y < m_WorldPositionZ + Size - Size / 15)
		{
			fern.push_back(Entity({ std::min(-(m_WorldPositionX + (int)Size / 15), -(rand() % (int)Size + m_WorldPositionX)), 0.f, std::min(-(m_WorldPositionZ + (int)Size / 15), -(rand() % (int)Size + m_WorldPositionZ)) }, { 0, 0, 0 }, 0.3f));
			grass.push_back(Entity({ -plant_pos.x, 0.f, -plant_pos.y }, { 0, 0, 0 }, 0.7f));
			plant_pos.y += Size / 15;
		}
		plant_pos.x += Size / 15;
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
			vertex.position = { -j / ((float)VertexCount - 1) * Size, (float)(rand() % 100) / 100.f, -i / ((float)VertexCount - 1) * Size };
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
