#pragma once

#include "Loader.h"
#include "Entity.h"		// Mesh.h

class Terrain
{
public:
	Terrain(const int & gridX, const int & gridZ, const unsigned int & textureID);
	Terrain(const Terrain & other);
	Terrain(const Terrain && other);
	~Terrain();

	std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>> GenerateForest(Shader * shader);
	std::map<std::tuple<Shader *, Model *, unsigned int *>, std::vector<Entity>> GenerateVegetation(Shader * shader);

	Terrain & operator=(Terrain & other);
	Terrain & operator=(Terrain && other);

	inline const unsigned int	GetGridX() const { return m_WorldPositionX; }
	inline const unsigned int	GetGridZ() const { return m_WorldPositionZ; }
	inline const Mesh &			GetMesh() const { return m_Mesh; }

private:
	static unsigned int Size, VertexCount;
	static std::map<std::string, Model *> TreesModels;
	static std::map<std::string, Model *> PlantsModels;
	Mesh m_Mesh;
	int m_WorldPositionX, m_WorldPositionZ;

	Mesh GenerateTerrain(const unsigned int & textureID);
};

