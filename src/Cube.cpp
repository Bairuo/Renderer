#include "Cube.h"
#include "BasicConfig.h"
#include "basicrender.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Animation.h"
#include "Object.h"
#include "Deferred.h"
#include "BasicMaterial.h"
#include <cmath>

const boost::shared_ptr<Material> Cube::kDefaultMaterial(
	new BASICMaterial(
    glm::vec3(1.0f, 0.5f, 0.31f),
    glm::vec3(1.0f, 0.5f, 0.31f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    32.0f)
);

#if defined(_WIN32)

#if defined(SHADOWMAP)
const GLchar Cube::kStandardVsPath[] = ".\\shaders\\3D_Standard\\standard_shadow.vs";
const GLchar Cube::kStandardFragPath[] = ".\\shaders\\3D_Standard\\standard_shadow.frag";
#else
const GLchar Cube::kStandardVsPath[] = ".\\shaders\\3D_Standard\\standard.vs";
const GLchar Cube::kStandardFragPath[] = ".\\shaders\\3D_Standard\\standard.frag";
#endif

#else

#if defined(SHADOWMAP)
const GLchar Cube::kStandardVsPath[] = "./shaders/3D_Standard/standard_shadow.vs";
const GLchar Cube::kStandardFragPath[] = "./shaders/3D_Standard/standard_shadow.frag";
#else
const GLchar Cube::kStandardVsPath[] = "./shaders/3D_Standard/standard.vs";
const GLchar Cube::kStandardFragPath[] = "./shaders/3D_Standard/standard.frag";
#endif

#endif

void Cube::render(Shader &shader)
{
    if(!active)
    {
        return;
    }

    shader.Use();

    shader.SetMat4("model", obj->transform->getMatrix());
    Camera::setMainCamera(&shader);
    Light::setLight(&shader);

	material->set(shader);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    shader.Stop();
}

void Cube::render()
{
    render(shader);
}

Cube::~Cube()
{
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
}

Cube::Cube(const boost::shared_ptr<Material> &material,
               const GLchar *vertexPath, const GLchar *fragmentPath)
               : material(material)
{
    shader = Shader(vertexPath, fragmentPath);

    shader.Use();
	material->set(shader);
    shader.Stop();

    VAO = getCubeVAO();
}

double Cube::getSphereBoundingRadius()
{
    double x = obj->transform->getPosX() * obj->transform->getScaleX();
    double y = obj->transform->getPosY() * obj->transform->getScaleY();
    double z = obj->transform->getPosZ() * obj->transform->getScaleZ();

    return sqrt(x * x + y * y + z * z);
}
