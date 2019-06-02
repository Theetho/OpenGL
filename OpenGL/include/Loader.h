#pragma once

#include "Model.h"

class Loader
{
public:
	Loader();
	~Loader();

	Model LoadToVao(const std::vector<float> & vertices, const std::vector<unsigned int> & indices);
	unsigned int LoadJPG(const std::string & texturePath);
	unsigned int LoadPNG(const std::string & texturePath);

private:
	std::vector<unsigned int> m_VAOs;
	std::vector<unsigned int> m_VBOs;
	std::vector<unsigned int> m_Textures;

	unsigned int LoadToVbo(const std::vector<unsigned int> & attributes, const std::vector<unsigned int> & dimensions, const std::vector<float> & data);
	unsigned int LoadToEbo(const std::vector<unsigned int> & data);
	unsigned int LoadTexture(const char * texturePath, const unsigned int & internalFormat, const unsigned int & format, const bool & flip = true);
};

