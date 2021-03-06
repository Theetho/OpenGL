#pragma once

#include "Model.h"

class Loader
{
public:
	Loader();
	~Loader();

	unsigned int LoadToVao(const std::vector<float> & vertices, const std::vector<unsigned int> & indices);
	unsigned int LoadJPG(const std::string & texturePath, const bool & flip = false);
	unsigned int LoadPNG(const std::string & texturePath, const bool & flip = false);

private:
	std::vector<unsigned int> m_VAOs;
	std::vector<unsigned int> m_VBOs;
	std::vector<unsigned int> m_Textures;

	unsigned int LoadToVbo(const std::vector<unsigned int> & attributes, const std::vector<unsigned int> & dimensions, const std::vector<float> & data);
	unsigned int LoadToEbo(const std::vector<unsigned int> & data);
	unsigned int LoadTexture(const char * texturePath, const bool & flip = true);
};

