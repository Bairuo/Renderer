#ifndef PBRMATERIAL_H_
#define PBRMATERIAL_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Material.h"

class Shader;

struct PBRMaterial : public Material
{
public:
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;

	PBRMaterial() = default;

	PBRMaterial(const glm::vec3 &albedo, const float metallic,
             const float roughness, float ao)
        :albedo(albedo),
		 metallic(metallic),
		 roughness(roughness),
		 ao(ao)
    {

    }

    ~PBRMaterial() = default;

	void set(Shader &shader);
};

#endif
