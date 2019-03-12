// Transform.h
// Author: Bairuo

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object;

class Transform
{
public:
    float getPosX() const;
    float getPosY() const;
    float getPosZ() const;

    float getScaleX() const;
    float getScaleY() const;
    float getScaleZ() const;

    glm::mat4 getMatrix() const;

    glm::vec3 transVec3;
    glm::vec3 scaleVec3;
    float angle;
    glm::vec3 axis;

    Object *obj = nullptr;

    void Update();

    Transform(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ,
            float angle = 0, glm::vec3 axis = glm::vec3(0, 0, 1.0f));
    Transform(float x, float y, float z, float s);
    Transform(float x, float y, float z, glm::vec3 scaleVec3);
    Transform(glm::vec3 transVec3, float s);
    Transform(glm::vec3 transVec3, glm::vec3 scaleVec3);
    Transform(glm::vec3 transVec3, glm::vec3 scaleVec3, float angle, glm::vec3 axis);
    Transform();

    static Transform Lerp(const Transform &start, const Transform &end, float k);

private:
    glm::mat4 transMatrix;
    glm::mat4 scaleMatrix;
    glm::mat4 rotateMatrix;
};

#endif // TRANSFORM_H_
