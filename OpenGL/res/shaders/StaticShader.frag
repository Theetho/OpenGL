#version 330 core

in vec2 from_vertex_texture_coordinates;
in vec4 from_vertex_color;

out vec4 out_color;

uniform sampler2D uniform_texture_1;
uniform sampler2D uniform_texture_2;

void main()
{
	out_color = mix(texture(uniform_texture_1, from_vertex_texture_coordinates), texture(uniform_texture_2, from_vertex_texture_coordinates), 0.2);
}