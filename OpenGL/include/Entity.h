#pragma once

#include "TexturedModel.h"
#include "Material.h"

class Entity
{
public:
	Entity(TexturedModel & texturedModel, const glm::vec3 & position, const glm::vec3 & rotation, const float & scale, const Material & material);
	~Entity();

	inline const TexturedModel &	GetTexturedModel() const { return m_TextureModel; }
	inline const glm::vec3			GetPosition() const { return m_Position; }
	inline const glm::vec3			GetRotation() const { return m_Rotation; }
	inline const glm::vec3			GetScaleVector() const { return glm::vec3(m_Scale, m_Scale, m_Scale); }
	inline const float				GetRotationX() const { return m_Rotation.x; }
	inline const float				GetRotationY() const { return m_Rotation.y; }
	inline const float				GetRotationZ() const { return m_Rotation.z; }
	inline const float				GetScale() const { return m_Scale; }
	inline const Material &			GetMaterial() const { return m_Material; }

	inline void						SetPosition(const glm::vec3 & position) { m_Position = position; }
	inline void						SetRotation(const glm::vec3 & rotation) { m_Rotation = rotation; }
	inline void						SetRotationX(const float & rotationX) { m_Rotation.x = rotationX; }
	inline void						SetRotationY(const float & rotationY) { m_Rotation.y = rotationY; }
	inline void						SetRotationZ(const float & rotationZ) { m_Rotation.z = rotationZ; }
	inline void						SetScale(const float & scale) { m_Scale = scale; }

private:
	TexturedModel & m_TextureModel;
	glm::vec3 m_Position, m_Rotation;
	float m_Scale;
	const Material & m_Material;
};


