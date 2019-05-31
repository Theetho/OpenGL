#include "pch.h"
#include "Light.h"

Light::Light(const glm::vec3 & position, const glm::vec4 & color, const float & ambientLighting) :
	m_Position(position),
	m_Color(color),
	m_AmbientLighting(ambientLighting)
{}

Light::~Light()
{}
