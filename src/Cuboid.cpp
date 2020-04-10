#include "Cuboid.h"
#include "BasicConfig.h"
#include "basicrender.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Animation.h"
#include "Object.h"
#include "Deferred.h"
#include <cmath>

const Material Cuboid::kDefaultMaterial(
    glm::vec3(1.0f, 0.5f, 0.31f),
    glm::vec3(1.0f, 0.5f, 0.31f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    32.0f
);

#if defined(_WIN32)

#if defined(SHADOWMAP)
const GLchar Cuboid::kStandardVsPath[] = ".\\shaders\\3D_Standard\\standard_s.vs";
const GLchar Cuboid::kStandardFragPath[] = ".\\shaders\\3D_Standard\\standard_s.frag";
#else
const GLchar Cuboid::kStandardVsPath[] = ".\\shaders\\3D_Standard\\standard.vs";
const GLchar Cuboid::kStandardFragPath[] = ".\\shaders\\3D_Standard\\standard.frag";
#endif

#else

#if defined(SHADOWMAP)
const GLchar Cuboid::kStandardVsPath[] = "./shaders/3D_Standard/standard_s.vs";
const GLchar Cuboid::kStandardFragPath[] = "./shaders/3D_Standard/standard_s.frag";
#else
const GLchar Cuboid::kStandardVsPath[] = "./shaders/3D_Standard/standard.vs";
const GLchar Cuboid::kStandardFragPath[] = "./shaders/3D_Standard/standard.frag";
#endif

#endif

void Cuboid::render(Shader &shader)
{
    if(!active)
    {
        return;
    }

    shader.Use();

    shader.SetMat4("model", obj->transform->getMatrix());
    Camera::setMainCamera(&shader);
    Light::setLight(&shader);

    setMaterial(&shader);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    shader.Stop();
}

void Cuboid::render()
{
    render(shader);
}

Cuboid::~Cuboid()
{
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
}


void Cuboid::setMaterial(Shader *shader)
{
    shader->Set3f("material.ambient", material.ambient);
    shader->Set3f("material.diffuse", material.diffuse);
    shader->Set3f("material.specular", material.specular);
    shader->SetFloat("material.shininess", material.shininess);
}

Cuboid::Cuboid(const Material &material,
               const GLchar *vertexPath, const GLchar *fragmentPath)
               : material(material)
{
    shader = Shader(vertexPath, fragmentPath);

    shader.Use();
    setMaterial(&shader);
    shader.Stop();

    VAO = getCubeVAO();
}

double Cuboid::getSphereBoundingRadius()
{
    double x = obj->transform->getPosX() * obj->transform->getScaleX();
    double y = obj->transform->getPosY() * obj->transform->getScaleY();
    double z = obj->transform->getPosZ() * obj->transform->getScaleZ();

    return sqrt(x * x + y * y + z * z);
}
