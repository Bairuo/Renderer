#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gPosLightSpace;
layout (location = 2) out vec4 gNormal;

layout (location = 3) out vec4 gAmbient;
layout (location = 4) out vec4 gDiffuse;
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
    gPosition.rgb = FragPos;
    gPosition.a = 1.0;

    gPosLightSpace = FragPosLightSpace;

    gNormal.rgb = normalize(vNormal);
    gNormal.a = 1.0;

    gAmbient.rgb = material.ambient;
    gAmbient.a = 1.0;

    gDiffuse.rgb = material.diffuse;
    gDiffuse.a = 1.0;

    gAlbedoSpec.rgb = material.specular;
    gAlbedoSpec.a = 1.0;
}
