#include<iostream>
#include "Transform.h"
#include "Object.h"
#include "Animation.h"

void Transform::update()
{
    if(obj != nullptr && obj->animation.get() != nullptr)
    {
		auto newTrans = new Transform(obj->animation->getTransform());
		newTrans->obj = obj;

        obj->transform.reset(newTrans);
    }
}

Transform::Transform(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ,
                 float angle, const glm::vec3 &axis, const glm::mat4 &parentMatrix)
    :angle(angle),
     axis(axis),
     transMatrix(glm::translate(glm::mat4(1), glm::vec3(posX, posY, posZ))),
     scaleMatrix(glm::scale(glm::mat4(1), glm::vec3(scaleX, scaleY, scaleZ))),
     rotateMatrix(glm::rotate(glm::mat4(1), angle, axis)),
     localMatrix(rotateMatrix * transMatrix * scaleMatrix),
     parentMatrix(parentMatrix),
     worldMatrix(localMatrix * parentMatrix)
{

}


Transform::Transform(float x, float y, float z, float s)
    :Transform(x, y, z, s, s, s)
{

}

Transform::Transform(float x, float y, float z, const glm::vec3 &scaleVec3)
    :Transform(x, y, z, scaleVec3.x, scaleVec3.y, scaleVec3.z)
{

}


Transform::Transform(const glm::vec3 &transVec3, float s)
    :Transform(transVec3.x, transVec3.y, transVec3.z, s, s, s)
{

}

Transform::Transform(const glm::vec3 &transVec3, const glm::vec3 &scaleVec3)
    :Transform(transVec3.x, transVec3.y, transVec3.z, scaleVec3.x, scaleVec3.y, scaleVec3.z)
{

}

Transform::Transform(const glm::vec3 &transVec3, const glm::vec3 &scaleVec3, float angle, const glm::vec3 &axis)
    :Transform(transVec3.x, transVec3.y, transVec3.z, scaleVec3.x, scaleVec3.y, scaleVec3.z, angle, axis)
{

}


Transform::Transform()
    :Transform(0, 0, 0, 1, 1, 1)
{

}

float Transform::getPosX() const
{
    return transMatrix[3][0];
}

float Transform::getPosY() const
{
    return transMatrix[3][1];
}

float Transform::getPosZ() const
{
    return transMatrix[3][2];
}

glm::vec3 Transform::getPosition() const
{
    return glm::vec3(transMatrix[3][0], transMatrix[3][1], transMatrix[3][2]);
}

float Transform::getScaleX() const
{
    return scaleMatrix[0][0];
}

float Transform::getScaleY() const
{
    return scaleMatrix[1][1];
}

float Transform::getScaleZ() const
{
    return scaleMatrix[2][2];
}

glm::vec3 Transform::getScale() const
{
    return glm::vec3(scaleMatrix[0][0], scaleMatrix[1][1], scaleMatrix[2][2]);
}

glm::mat4 Transform::getMatrix() const
{
    return worldMatrix;
}

void Transform::setParentMatrix(const glm::mat4 &parentMatrix)
{
    this->parentMatrix = parentMatrix;

    worldMatrix = localMatrix * parentMatrix;
}

Transform Transform::Lerp(const Transform &start, const Transform &end, float k)
{
    float posX = start.getPosX() + (end.getPosX() - start.getPosX()) * k;
    float posY = start.getPosY() + (end.getPosY() - start.getPosY()) * k;
    float posZ = start.getPosZ() + (end.getPosZ() - start.getPosZ()) * k;

    float scaleX = start.getScaleX() + (end.getScaleX() - start.getScaleX()) * k;
    float scaleY = start.getScaleY() + (end.getScaleY() - start.getScaleY()) * k;
    float scaleZ = start.getScaleZ() + (end.getScaleZ() - start.getScaleZ()) * k;

    float angle = start.angle + (end.angle - start.angle) * k;

    glm::vec3 axis = start.axis + (end.axis - start.axis) * k;

    return Transform(posX, posY, posZ, scaleX, scaleY, scaleZ, angle, axis, start.parentMatrix);
}
