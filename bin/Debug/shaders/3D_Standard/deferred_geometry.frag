#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gPosLightSpace;
layout (location = 2) out vec3 gNormal;

layout (location = 3) out vec3 gAmbient
layout (location = 4) out vec3 gDiffuse
layout (location = 5) out vec4 gAlbedoSpec;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

in vec3 FragPos;
in vec3 vNormal;
in vec4 FragPosLightSpace;

void main()
{
    gPosition = FragPos;

    gPosLightSpace = FragPosLightSpace;

    gNormal = normalize(vNormal);

    gAmbient = material.ambient;

    gDiffuse = material.diffuse;

    gAlbedoSpec.rgb = material.specular;

    gAlbedoSpec.a = material.shininess;
}
