#version 330 core

in vec4 vcolor;

out vec4 color;

uniform float radius;
uniform float recWidth;
uniform float recHeight;

uniform int windowWidth;
uniform int windowHeight;
uniform vec4 roundColor;

uniform mat4 transform;

vec4 worldPos = vec4(0, 0, 0, 1);

float GetObjectx(float windowx)
{
    return (windowx - windowWidth / 2) / (windowWidth / 2) - worldPos.x;
}

float GetObjecty(float windowy)
{
    return (windowy - windowHeight / 2) / (windowHeight / 2) - worldPos.y;
}

void main()
{
    worldPos = transform * worldPos;
    float x = abs(GetObjectx(gl_FragCoord.x)), y = abs(GetObjecty(gl_FragCoord.y));
    float x0 = recWidth / 2 - radius, y0 = recHeight / 2 - radius;
    float l;

    if(x > x0 && y > y0)
    {
        l = length(vec2(x - x0, y - y0));
        if(l > radius)
            color = roundColor;
        else
            color = vcolor;
    }
    else
        color = vcolor;
}
