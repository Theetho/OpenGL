#version 330 core

layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coordinate;

out vec3 from_vertex_light_vector;
out vec3 from_vertex_normal;
out vec2 from_vertex_texture_coordinate;
out vec3 from_vertex_camera_vector;

uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;
uniform vec3 uniform_light_position;
uniform bool uniform_fake_lighting;

void main()
{
	vec4 world_position = uniform_model * vec4(in_vertex, 1.0);

	gl_Position = uniform_projection * uniform_view * world_position;
	
	from_vertex_light_vector = normalize(uniform_light_position - world_position.xyz);
	
	if (uniform_fake_lighting)
		from_vertex_normal = normalize(mat3(transpose(inverse(uniform_model))) * vec3(0, 1, 0));
	else
		from_vertex_normal = normalize(mat3(transpose(inverse(uniform_model))) * in_normal);
	
	from_vertex_texture_coordinate = in_texture_coordinate;

	from_vertex_camera_vector = normalize(vec3(inverse(uniform_view) * vec4(0.0, 0.0, 0.0, 1.0) - world_position).xyz);
}