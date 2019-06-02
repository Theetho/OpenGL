#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Attenuation part
	float constant;
	float linear;
	float quadratic;
};

in vec2 from_vertex_texture_coordinates;
in vec3 from_vertex_normal;
in vec3 from_vertex_vertex;

out vec4 out_color;

uniform vec3 uniform_camera_position;
uniform Material uniform_material;
uniform Light uniform_light;

void main()
{
	// Ambient
	vec3 ambient = uniform_light.ambient * vec3(texture(uniform_material.diffuse, from_vertex_texture_coordinates));

	// Diffuse
	vec3 light_direction = normalize(uniform_light.position - from_vertex_vertex);
	float diffuse_strengh = max(dot(light_direction, from_vertex_normal), 0.0);
	vec3 diffuse =  uniform_light.diffuse * diffuse_strengh * vec3(texture(uniform_material.diffuse, from_vertex_texture_coordinates));

	// Specular
	vec3 camera_direction = normalize(from_vertex_vertex - uniform_camera_position);
	vec3 reflect_direction = reflect(light_direction, from_vertex_normal);
	float specular_strengh = pow(max(dot(camera_direction, reflect_direction), 0.0), uniform_material.shininess);
	vec3 specular = uniform_light.specular * specular_strengh * vec3(texture(uniform_material.specular, from_vertex_texture_coordinates));

	// Spotlight
	float theta = dot(light_direction, normalize(- uniform_light.direction));
	float epsilon = uniform_light.cutOff - uniform_light.outerCutOff;
	float intensity = clamp((theta - uniform_light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	// Attenuation
	float distance = length(uniform_light.position - from_vertex_vertex);
	float attenuation = 1.0 / (uniform_light.constant + uniform_light.linear * distance + uniform_light.quadratic * pow(distance, 2));

	out_color = vec4((ambient + diffuse + specular ) * attenuation, 1.0);
}
