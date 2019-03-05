#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <GL/glew.h>

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Material();

    Material(const glm::vec3 &ambient, const glm::vec3 &diffuse,
             const glm::vec3 &specular, float shininess)
        :ambient(ambient),
         diffuse(diffuse),
         specular(specular),
         shininess(shininess)
    {

    }
};

#endif
