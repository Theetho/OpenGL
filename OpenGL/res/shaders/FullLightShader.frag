#version 330 core

struct Material
{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	float shininess;
};

in vec2 from_vertex_texture_coordinate;

out vec4 out_color;

uniform Material material;

void main()
{
	out_color = texture(material.texture_diffuse0, from_vertex_texture_coordinate);
	out_color += texture(material.texture_specular0, from_vertex_texture_coordinate);
}