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
    float angle;
    glm::vec3 axis;
    Object *obj = nullptr;

    float getPosX() const;
    float getPosY() const;
    float getPosZ() const;

    glm::vec3 getPosition() const;

    float getScaleX() const;
    float getScaleY() const;
    float getScaleZ() const;

    glm::vec3 getScale() const;

    glm::mat4 getMatrix() const;

    void setParentMatrix(const glm::mat4 &parentMatrix);

    void update();

    Transform(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ,
            float angle = 0, const glm::vec3 &axis = glm::vec3(0, 0, 1.0f), const glm::mat4 &parentMatrix = glm::mat4(1));
    Transform(float x, float y, float z, float s);
    Transform(float x, float y, float z, const glm::vec3 &scaleVec3);
    Transform(const glm::vec3 &transVec3, float s);
    Transform(const glm::vec3 &transVec3, const glm::vec3 &scaleVec3);
    Transform(const glm::vec3 &transVec3, const glm::vec3 &scaleVec3, float angle, const glm::vec3 &axis);
    Transform();

    static Transform Lerp(const Transform &start, const Transform &end, float k);

private:
    glm::mat4 transMatrix;
    glm::mat4 scaleMatrix;
    glm::mat4 rotateMatrix;

    glm::mat4 localMatrix;
    glm::mat4 parentMatrix;
    glm::mat4 worldMatrix;
};

#endif // TRANSFORM_H_
