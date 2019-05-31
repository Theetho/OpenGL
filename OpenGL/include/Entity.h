#pragma once

#include "TexturedModel.h"

class Entity
{
public:
	Entity(TexturedModel & texturedModel, const glm::vec3 & position, const glm::vec3 & rotation, const float & scale);
	~Entity();

	inline const TexturedModel &	GetTexturedModel() const { return m_TextureModel; }
	inline const glm::vec3			GetPosition() const { return m_Position; }
	inline const glm::vec3			GetRotation() const { return m_Rotation; }
	inline const glm::vec3			GetScaleVector() const { return glm::vec3(m_Scale, m_Scale, m_Scale); }
	inline const float				GetRotationX() const { return m_Rotation.x; }
	inline const float				GetRotationY() const { return m_Rotation.y; }
	inline const float				GetRotationZ() const { return m_Rotation.z; }
	inline const float				GetScale() const { return m_Scale; }

private:
	TexturedModel & m_TextureModel;
	glm::vec3 m_Position, m_Rotation;
	float m_Scale;
};


