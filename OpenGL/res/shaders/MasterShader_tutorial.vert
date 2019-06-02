#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;

void main()
{
    FragPos = vec3(uniform_model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(uniform_model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = uniform_projection * uniform_view * vec4(FragPos, 1.0);
}