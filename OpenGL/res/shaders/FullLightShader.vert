#version 330 core

layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coordinate;

out vec2 from_vertex_texture_coordinate;

uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;

void main()
{
	gl_Position = uniform_projection * uniform_view * uniform_model * vec4(in_vertex, 1.0);
	from_vertex_texture_coordinate = in_texture_coordinate;
}