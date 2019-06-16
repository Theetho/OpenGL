#version 330 core

struct Material
{
	bool texture;
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	vec3 ambient0;
	vec3 diffuse0;
	vec3 specular0;
	float shininess;
};

struct Sun
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 from_vertex_light_vector;
in vec3 from_vertex_normal;
in vec2 from_vertex_texture_coordinate;
in vec3 from_vertex_camera_vector;

out vec4 out_color;

uniform Sun sun;
uniform Material material;

void main()
{	
	vec4 texture_color;
	if (material.texture)
	{
		texture_color = texture(material.texture_diffuse0, from_vertex_texture_coordinate);
		if (texture_color.a < 0.5)
			discard;
	}
	else
		texture_color = vec4(material.diffuse0, 1.0);
	// Diffuse
	float brightness = dot(from_vertex_normal, from_vertex_light_vector);
	vec3 ambient;
	if (material.texture)
		ambient = sun.ambient;
	else
		ambient = sun.ambient * material.ambient0;

	vec3 diffuse_factor = max(vec3(brightness, brightness, brightness), ambient);
	vec3 diffuse = diffuse_factor * sun.diffuse;

	out_color = vec4(diffuse, 1.0) * texture_color;

	// Specular
	vec3 light_direction = - from_vertex_light_vector;
	vec3 reflected_light_direction = reflect(light_direction, from_vertex_normal);
	float shininess = pow(max(dot(reflected_light_direction, from_vertex_camera_vector), 0.0), material.shininess);

	vec3 specular_factor = vec3(shininess, shininess, shininess);
	vec3 specular = specular_factor * sun.specular;

	if (material.texture)
		out_color += vec4(specular, 1.0) * texture(material.texture_specular0, from_vertex_texture_coordinate);
	else
		out_color += vec4(specular, 1.0) * vec4(material.specular0, 1.0);
}