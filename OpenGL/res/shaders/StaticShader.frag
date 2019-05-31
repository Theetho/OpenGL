#version 330 core

out vec4 out_color;

uniform vec4 uniform_cube_color;
uniform vec4 uniform_light_color;

void main()
{
	out_color = uniform_cube_color * uniform_light_color;
}