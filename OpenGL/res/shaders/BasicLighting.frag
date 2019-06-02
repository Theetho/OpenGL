#version 330 core

in vec3 from_vertex_normal;
in vec3 from_vertex_vertex;

out vec4 out_color;

uniform vec4 uniform_cube_color;
uniform vec4 uniform_light_color;
uniform vec3 uniform_light_position;
uniform vec3 uniform_camera_position;
uniform float uniform_ambient_lighting_factor;
uniform float uniform_specular_lighting_factor;

void main()
{
	vec3 light_direction = normalize(uniform_light_position - from_vertex_vertex);
	vec3 camera_direction = normalize(from_vertex_vertex - uniform_camera_position);
	vec3 reflect_direction = reflect(light_direction, from_vertex_normal);
	float specular = pow(max(dot(camera_direction, reflect_direction), 0.0), 32);
	float brightness = max(dot(light_direction, from_vertex_normal), uniform_ambient_lighting_factor);
	out_color = uniform_cube_color * uniform_light_color * (brightness + specular * uniform_specular_lighting_factor);
}