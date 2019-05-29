#pragma once

#include "Model.h"

class TexturedModel
{
public:
	TexturedModel(const Model & model, const std::vector<unsigned int> & textures);
	~TexturedModel();
	inline const Model &					 GetModel() const { return m_Model; }
	inline const unsigned int				 GetTexture(const unsigned int & index = 0) const { return m_Textures[index]; }
	inline const std::vector<unsigned int> & GetTextures() const { return m_Textures; }

private:
	Model m_Model;
	std::vector<unsigned int> m_Textures;
};

