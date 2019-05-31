#include "pch.h"
#include "Loader.h"

Loader::Loader()
{
}

Loader::~Loader()
{
	for (auto & vao : m_VAOs)
		glDeleteVertexArrays(1, &vao);
	for (auto & vbo : m_VBOs)
		glDeleteBuffers(1, &vbo);
}

unsigned int Loader::LoadToVao(const std::vector<float>& vertices, const std::vector<unsigned int> & indices)
{
	unsigned int vao_id;
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	LoadToEbo(indices);
	LoadToVbo({ 0, 1 }, { 3, 2 }, vertices);
	glBindVertexArray(0);
	m_VAOs.push_back(vao_id);
	return vao_id;
}

unsigned int Loader::LoadJPG(const std::string & texturePath)
{
	return LoadTexture(("res/textures/" + texturePath + ".jpg").c_str(), GL_RGB, GL_RGB, false);
}

unsigned int Loader::LoadPNG(const std::string & texturePath)
{	
	return LoadTexture(("res/textures/" + texturePath + ".png").c_str(), GL_RGB, GL_RGBA);
}

unsigned int Loader::LoadToVbo(const std::vector<unsigned int> & attributes, const std::vector<unsigned int> & dimensions, const std::vector<float>& data)
{
	unsigned int vbo_id, element_per_vertex = 0, offset = 0;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	for (auto & dimension : dimensions)
		element_per_vertex += dimension;
	unsigned int size_of_vertex = element_per_vertex * sizeof(float);
	for (unsigned int i = 0; i < attributes.size(); ++i)
	{
		glVertexAttribPointer(attributes[i], dimensions[i], GL_FLOAT, GL_FALSE, size_of_vertex, (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(attributes[i]);
		offset += dimensions[i];
	}
	m_VBOs.push_back(vbo_id);
	return vbo_id;
}

unsigned int Loader::LoadToEbo(const std::vector<unsigned int> & data)
{
	unsigned int ebo_id;
	glGenBuffers(1, &ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), GL_STATIC_DRAW);
	// EBOs are like VBOs (they are buffers), so we store them in the same vector
	m_VBOs.push_back(ebo_id);
	return ebo_id;
}

unsigned int Loader::LoadTexture(const char * texturePath, const unsigned int & internalFormat, const unsigned int & format, const bool & flip)
{
	stbi_set_flip_vertically_on_load(flip);
	int width, height, channels;
	unsigned char *data = stbi_load(texturePath, &width, &height, &channels, 0);
	unsigned int texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load the texture\n";
	}
	stbi_image_free(data);
	m_Textures.push_back(texture_id);
	return texture_id;
}
