#version 330 core

layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coordinate;

out vec3 from_vertex_vertex;
out vec3 from_vertex_normal;
out vec2 from_vertex_texture_coordinate;

uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;
uniform bool uniform_fake_lighting;

void main()
{
	gl_Position = uniform_projection * uniform_view * uniform_model * vec4(in_vertex, 1.0);
	from_vertex_vertex = vec3(uniform_model * vec4(in_vertex, 1.0)).xyz;
	//if (uniform_fake_lighting)
		//from_vertex_normal = normalize(mat3(transpose(inverse(uniform_model))) * vec3(0, 1, 0));
	//else
		from_vertex_normal = normalize(mat3(transpose(inverse(uniform_model))) * in_normal);
	from_vertex_texture_coordinate = in_texture_coordinate;
}