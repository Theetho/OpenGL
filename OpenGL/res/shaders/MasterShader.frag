#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};


struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#define NB_POINT_LIGHT 4

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 camera_direction);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragment_position, vec3 camera_direction);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragment_position, vec3 camera_direction);


in vec3 from_vertex_vertex;
in vec3 from_vertex_normal;
in vec2 from_vertex_texture_coordinate;

out vec4 out_color;

uniform vec3 camera_position;
uniform Material material;
uniform DirectionalLight directional_light;
uniform PointLight point_light[NB_POINT_LIGHT];
uniform SpotLight spot_light;

void main()
{
	vec3 camera_direction = normalize(camera_position - from_vertex_vertex);

	vec3 color = CalculateDirectionalLight(directional_light, from_vertex_normal, camera_direction);

	for (int i = 0; i < NB_POINT_LIGHT; ++i)
	{
		color += CalculatePointLight(point_light[i], from_vertex_normal, from_vertex_vertex, camera_direction);
	}

	color += CalculateSpotLight(spot_light, from_vertex_normal, from_vertex_vertex, camera_direction);

	out_color = vec4(color, 1.0);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 camera_direction)
{
	vec3 light_direction = normalize(- light.direction);
	// Diffuse
	float diffuse_factor = max(dot(normal, light_direction), 0.0);
	// Specular
	vec3 reflect_direction = reflect(- light_direction, normal);
	float specular_factor = pow(max(dot(camera_direction, reflect_direction), 0.0), material.shininess);
	// Final result
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, from_vertex_texture_coordinate));
	vec3 diffuse = light.diffuse * diffuse_factor * vec3(texture(material.diffuse, from_vertex_texture_coordinate));
	vec3 specular = light.specular * specular_factor * vec3(texture(material.specular, from_vertex_texture_coordinate));

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragment_position, vec3 camera_direction)
{
	vec3 light_direction = normalize(light.position - fragment_position);
	// Diffuse
	float diffuse_factor = max(dot(normal, light_direction), 0.0);
	// Specular
	vec3 reflect_direction = reflect(- light_direction, normal);
	float specular_factor = pow(max(dot(camera_direction, reflect_direction), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position - fragment_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	//Final result
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, from_vertex_texture_coordinate)) * attenuation;
	vec3 diffuse = light.diffuse * diffuse_factor * vec3(texture(material.diffuse, from_vertex_texture_coordinate)) * attenuation;
	vec3 specular = light.specular * specular_factor * vec3(texture(material.specular, from_vertex_texture_coordinate)) * attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragment_position, vec3 camera_direction)
{
	vec3 light_direction = normalize(light.position - fragment_position);
	// Diffuse
	float diffuse_strengh = max(dot(from_vertex_normal, light_direction), 0.0);
	// Specular
	vec3 reflect_direction = reflect(-light_direction, from_vertex_normal);
	float specular_strengh = pow(max(dot(camera_direction, reflect_direction), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position - from_vertex_vertex);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	// Spotlight
	float theta = dot(light_direction, normalize(- light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, from_vertex_texture_coordinate));
	vec3 diffuse =  light.diffuse * diffuse_strengh * vec3(texture(material.diffuse, from_vertex_texture_coordinate));
	vec3 specular = light.specular * specular_strengh * vec3(texture(material.specular, from_vertex_texture_coordinate));
	
	ambient *= intensity * attenuation; 
	diffuse *= intensity * attenuation;
	specular *= intensity * attenuation;

	return (ambient + diffuse + specular);
}


