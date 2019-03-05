#version 330 core

layout (location = 0) in vec3 position;

out vec4 vcolor;

uniform vec4 setcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vcolor = setcolor;

    gl_Position = projection * view * model * vec4(position, 1.0f);
}
