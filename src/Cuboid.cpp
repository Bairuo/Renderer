#include "Cuboid.h"
#include "ConfigUtilities.h"
#include "basicrender.h"
#include "Camera.h"
#include "Light.h"
#include "Posture.h"
#include "Animation.h"
#include "Object.h"
#include "Deferred.h"
#include <cmath>

const Material Cuboid::defaultMaterial(
    glm::vec3(1.0f, 0.5f, 0.31f),
    glm::vec3(1.0f, 0.5f, 0.31f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    32.0f
);

void Cuboid::Update()
{
    if(!active)
    {
        return;
    }

    // set
    if(GBufferMode)
    {
        deferredGeometryShader.Use();

        deferredGeometryShader.SetMat4("model", obj->posture->getMatrix());
        Camera::setMainCamera(&deferredGeometryShader);
        setMaterial(&deferredGeometryShader);
    }
    else if(Light::depthMode && castShadow)
    {
        Light::startRenderDepth(obj->posture.get());
    }
    else
    {
        shader.Use();

        if(obj->animation.get() != nullptr)
        {
            obj->posture.reset(new Posture(obj->animation->getPosture()));
        }

        shader.SetMat4("model", obj->posture->getMatrix());
        Camera::setMainCamera(&shader);
        Light::setLight(&shader);
    }

    // draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // stop shader
    if(Light::depthMode && castShadow)
    {
        Light::stopRenderDepth();
    }
    else if(GBufferMode)
    {
        deferredGeometryShader.Stop();
    }
    else
    {
        shader.Stop();
    }
}

Cuboid::~Cuboid()
{
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
}


void Cuboid::setMaterial(Shader *shader)
{
    shader->Use();

    shader->Set3f("material.ambient", material.ambient);
    shader->Set3f("material.diffuse", material.diffuse);
    shader->Set3f("material.specular", material.specular);
    shader->SetFloat("material.shininess", material.shininess);

    shader->Stop();
}

Cuboid::Cuboid(const Material &material,
               const GLchar *vertexPath, const GLchar *fragmentPath)
               : material(material)
{
    shader = Shader(vertexPath, fragmentPath);

    setMaterial(&shader);

    VAO = getCubeVAO();
}

double Cuboid::getSphereBoundingRadius()
{
    double x = obj->posture->getPosX() * obj->posture->getScaleX();
    double y = obj->posture->getPosY() * obj->posture->getScaleY();
    double z = obj->posture->getPosZ() * obj->posture->getScaleZ();

    return sqrt(x * x + y * y + z * z);
}
