#version 330 core

layout (location = 0) in vec3 in_vertex;

uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;

void main()
{
	gl_Position = uniform_projection * uniform_view * uniform_model * vec4(in_vertex, 1.0);
}