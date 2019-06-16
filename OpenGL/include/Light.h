#pragma once
/* Coefficient value for a given distance
	7		1.0		0.7		1.8
	13		1.0		0.35	0.44
	20		1.0		0.22	0.20
	32		1.0		0.14	0.07
	50		1.0		0.09	0.032
	65		1.0		0.07	0.017
	100		1.0		0.045	0.0075
	160		1.0		0.027	0.0028
	200		1.0		0.022	0.0019
	325		1.0		0.014	0.0007
	600		1.0		0.007	0.0002
	3250	1.0		0.0014	0.000007	*/
// For a "100 meters" long light ray
static const float CONSTANT = 1.0f;
static const float LINEAR = 0.35f;
static const float QUADRATIC = 0.44f;

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
			   const float & _linear = LINEAR, const float & _quadratic = QUADRATIC, const float & _constant = CONSTANT);
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
			  const glm::vec3 &  _diffuse, const glm::vec3 & _specular, const float & _linear = LINEAR, const float & _quadratic = QUADRATIC, const float & _constant = CONSTANT);
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