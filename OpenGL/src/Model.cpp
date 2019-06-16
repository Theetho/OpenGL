#include "pch.h"
#include "Model.h"

Model::Model(const std::string & path, const bool & hasTransparency, const bool & useFakeLighting)
{
	LoadModel(path);
	for (Mesh & mesh : m_Mesh)
	{
		for (Texture & texture : mesh.m_Textures)
		{
			texture.useFakeLight = useFakeLighting;
			texture.hasTransparency = hasTransparency;
		}
	}
}

Model::Model(std::vector<Mesh>& mesh) :
	m_Mesh(mesh)
{}

Model::~Model()
{
	for (Mesh & mesh : m_Mesh)
	{
		unsigned int vao = mesh.GetVao(), vbo = mesh.GetVbo(), ebo = mesh.GetEbo(); 
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
}

void Model::LoadModel(const std::string & path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	m_Directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		m_Mesh.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
		ProcessNode(node->mChildren[i], scene);
}

Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	// Vertices processing
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// Positions
		vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

		// Normals
		vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

		// Texture coordinates
		if (mesh->mTextureCoords[0])
			vertex.texture_coordinate = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		else
			vertex.texture_coordinate = { 0.0, 0.0 };

		// Tangents
		if (mesh->mTangents)
			vertex.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };

		// Bitangents
		if (mesh->mBitangents)
			vertex.bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };

		vertices.push_back(vertex);
	}
	// Indices processing
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}
	// Materials processing
	aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];

	// Textures
	std::vector<Texture> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

	std::vector<Texture> specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

	std::vector<Texture> normal_maps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

	std::vector<Texture> height_maps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), height_maps.begin(), height_maps.end());

	// Materials
	std::vector<Material> materials = LoadMaterials(material);

	return Mesh(vertices, textures, materials, indices);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial * material, const aiTextureType & type, const std::string & typeName)
{
	std::vector<Texture> textures;
	
	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_LoadedTextures.size(); ++j)
		{
			if (std::strcmp(m_LoadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_LoadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), m_Directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_LoadedTextures.push_back(texture);
		}
	}
	return textures;
}

std::vector<Material> Model::LoadMaterials(aiMaterial * material)
{
	std::vector<Material> materials;
	Material mat;

	aiColor4D ambient(0, 0, 0, 0), diffuse(0, 0, 0, 0), specular(0, 0, 0, 0), emissive(0, 0, 0, 0);
	aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);
	aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
	aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive);
	// Black or loaded color
	mat.ambient		= glm::vec3(ambient.r, ambient.g, ambient.b);
	mat.diffuse		= glm::vec3(diffuse.r, diffuse.g, diffuse.b);
	mat.specular	= glm::vec3(specular.r, specular.g, specular.b);
	mat.emissive	= glm::vec3(emissive.r, emissive.g, emissive.b);

	materials.push_back(mat);

	return materials;
}

unsigned int TextureFromFile(const char * path, const std::string & directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
