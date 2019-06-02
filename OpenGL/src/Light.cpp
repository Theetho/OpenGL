#include "pch.h"
#include "Light.h"

Light::Light(const glm::vec3 & _ambient, const glm::vec3 & _diffuse, const glm::vec3 & _specular) :
	ambient(_ambient),
	diffuse(_diffuse),
	specular(_specular)
{
}

DirectionalLight::DirectionalLight(const glm::vec3 &  _direction, const glm::vec3 & _ambient, const glm::vec3 & _diffuse, const glm::vec3 & _specular) :
	Light(_ambient, _diffuse, _specular),
	direction(_direction)
{
}

PointLight::PointLight(const glm::vec3 &  _position, const glm::vec3 & _ambient, const glm::vec3 & _diffuse, const glm::vec3 & _specular,
					   const float & _linear, const float & _quadratic, const float & _constant) :
	Light(_ambient, _diffuse, _specular),
	position(_position),
	constant(_constant),
	linear(_linear),
	quadratic(_quadratic)
{
}

SpotLight::SpotLight(const glm::vec3 &  _position, const glm::vec3 & _direction, const float & _cutOff, const float & _outerCutOff, const glm::vec3 & _ambient,
					 const glm::vec3 &  _diffuse, const glm::vec3 & _specular, const float & _linear, const float & _quadratic, const float & _constant) :
	DirectionalLight(_direction, _ambient, _diffuse, _specular),
	position(_position),
	cutOff(_cutOff),
	outerCutOff(_outerCutOff),
	constant(_constant),
	linear(_linear),
	quadratic(_quadratic)
{
}

