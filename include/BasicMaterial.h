#ifndef BASICMATERIAL_H_
#define BASICMATERIAL_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Material.h"

class Shader;

struct BASICMaterial : public Material
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

	BASICMaterial() = default;

	BASICMaterial(const glm::vec3 &ambient, const glm::vec3 &diffuse,
             const glm::vec3 &specular, float shininess)
        :ambient(ambient),
         diffuse(diffuse),
         specular(specular),
         shininess(shininess)
    {

    }

    ~BASICMaterial() = default;

	void set(Shader &shader);
};

#endif
