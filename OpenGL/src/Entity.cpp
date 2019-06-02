#include "pch.h"
#include "Entity.h"


Entity::Entity(TexturedModel & texturedModel, const glm::vec3 & position, const glm::vec3 & rotation, const float & scale, const Material & material) :
	m_TextureModel(texturedModel),
	m_Position(position),
	m_Rotation(rotation),
	m_Scale(scale),
	m_Material(material)
{}


Entity::~Entity()
{}
