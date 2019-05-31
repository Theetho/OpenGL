#pragma once

class Light
{
public:
	Light(const glm::vec3 & position, const glm::vec4 & color, const float & ambientLighting);
	~Light();

	inline const glm::vec3 & GetPosition() const { return m_Position; }
	inline const glm::vec4 & GetColor() const { return m_Color; }
	inline const float GetAmbientLighting() const { return m_AmbientLighting; }

private:
	glm::vec3 m_Position;
	glm::vec4 m_Color;
	float m_AmbientLighting;
};

