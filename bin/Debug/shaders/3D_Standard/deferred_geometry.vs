#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 vNormal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    FragPos = vec3(model * vec4(position, 1.0));

    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    vNormal = normal;
}
