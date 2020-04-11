#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    WorldPos = vec3(model * vec4(position, 1.0));

    Normal = normalize(transpose(inverse(mat3(model))) * normal);
}
