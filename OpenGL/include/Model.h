#pragma once

#include "Mesh.h"

unsigned int TextureFromFile(const char *path, const std::string &directory);

class Model
{
public:
	Model(const std::string & path, const bool & hasTransparency = false, const bool & useFakeLighting = false);
	Model(std::vector<Mesh> & mesh);
	~Model();

	inline std::vector<Mesh> &	GetMesh() { return m_Mesh; }

private:
	std::vector<Mesh> m_Mesh;
	std::vector<Texture> m_LoadedTextures;
	std::string m_Directory;

	void LoadModel(const std::string & path);
	void ProcessNode(aiNode * node, const aiScene * scene);
	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial * material, const aiTextureType & type, const std::string & typeName);
	std::vector<Material> LoadMaterials(aiMaterial * material);
};


