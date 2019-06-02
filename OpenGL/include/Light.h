#pragma once

struct Light
{
	Light(const glm::vec3 & _ambient, const glm::vec3 & _diffuse, const glm::vec3 & _specular);
	~Light() {}

	// Colors
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct DirectionalLight : public Light
{
	DirectionalLight(const glm::vec3 & _direction, const glm::vec3 & _ambient, const glm::vec3 & _diffuse, const glm::vec3 & _specular);
	~DirectionalLight() {}

	glm::vec3 direction;
};

struct PointLight : public Light
{
	PointLight(const glm::vec3 &  _position, const glm::vec3 & _ambient, const glm::vec3 & _diffuse, const glm::vec3 & _specular,
			   const float & _linear = 0.09f, const float & _quadratic = 0.032f, const float & _constant = 1.f);
	~PointLight() {}

	glm::vec3 position;

	// Attenuation factors
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight : public DirectionalLight
{
	SpotLight(const glm::vec3 &  _position, const glm::vec3 & _direction, const float & _cutOff, const float & _outerCutOff, const glm::vec3 & _ambient,
			  const glm::vec3 &  _diffuse, const glm::vec3 & _specular, const float & _linear = 0.09f, const float & _quadratic = 0.032f, const float & _constant = 1.f);
	~SpotLight() {}

	// SpotLight shape
	glm::vec3 position;
	float cutOff;
	float outerCutOff;

	// Attenuation factors
	float constant;
	float linear;
	float quadratic;
};