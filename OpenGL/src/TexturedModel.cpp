#include "pch.h"
#include "TexturedModel.h"

TexturedModel::TexturedModel(const Model & model, const std::vector<unsigned int> & textures) :
	m_Model(model),
	m_Textures(textures)
{}

TexturedModel::~TexturedModel()
{}
