// Cuboid.h
// Author: Bairuo

#ifndef CUBOID_H_
#define CUBOID_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

#include "Color.h"
#include "Shader.h"
#include "Material.h"
#include "Renderer.h"

class Cuboid : public Renderer
{
public:
    bool castShadow = true;

    virtual double getSphereBoundingRadius();

    virtual void Update();

    virtual ~Cuboid();

    Cuboid(const Material &material = defaultMaterial,
           const GLchar *vertexPath = standardVsPath, const GLchar *fragmentPath = standardFragPath);


private:
    GLuint VAO;

    Shader shader;

    bool active = true;

    static const GLchar standardVsPath[];
    static const GLchar standardFragPath[];
    static const Material defaultMaterial;
};


#endif // CUBOID_H_
