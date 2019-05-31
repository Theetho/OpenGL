#version 330 core

out vec4 out_color;

uniform vec3 uniform_cube_color;
uniform vec3 uniform_light_color;

void main()
{
	out_color = vec4(uniform_cube_color * uniform_light_color, 1.0);
}