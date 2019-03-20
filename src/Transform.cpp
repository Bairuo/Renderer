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
                 float angle, glm::vec3 axis)
    :transVec3(posX, posY, posZ),
     scaleVec3(scaleX, scaleY, scaleZ),
     angle(angle),
     axis(axis),
     transMatrix(glm::translate(glm::mat4(1), transVec3)),
     scaleMatrix(glm::scale(glm::mat4(1), scaleVec3)),
     rotateMatrix(glm::rotate(glm::mat4(1), angle, axis))
{

}


Transform::Transform(float x, float y, float z, float s)
    :Transform(x, y, z, s, s, s)
{

}

Transform::Transform(float x, float y, float z, glm::vec3 scaleVec3)
    :Transform(x, y, z, scaleVec3.x, scaleVec3.y, scaleVec3.z)
{

}


Transform::Transform(glm::vec3 transVec3, float s)
    :Transform(transVec3.x, transVec3.y, transVec3.z, s, s, s)
{

}

Transform::Transform(glm::vec3 transVec3, glm::vec3 scaleVec3)
    :Transform(transVec3.x, transVec3.y, transVec3.z, scaleVec3.x, scaleVec3.y, scaleVec3.z)
{

}

Transform::Transform(glm::vec3 transVec3, glm::vec3 scaleVec3, float angle, glm::vec3 axis)
    :Transform(transVec3.x, transVec3.y, transVec3.z, scaleVec3.x, scaleVec3.y, scaleVec3.z, angle, axis)
{

}


Transform::Transform()
    :Transform(0, 0, 0, 1, 1, 1)
{

}

float Transform::getPosX() const
{
    return transVec3.x;
}

float Transform::getPosY() const
{
    return transVec3.y;
}

float Transform::getPosZ() const
{
    return transVec3.z;
}

float Transform::getScaleX() const
{
    return scaleVec3.x;
}

float Transform::getScaleY() const
{
    return scaleVec3.y;
}

float Transform::getScaleZ() const
{
    return scaleVec3.z;
}

glm::mat4 Transform::getMatrix() const
{
    return rotateMatrix * transMatrix * scaleMatrix;
}

Transform Transform::Lerp(const Transform &start, const Transform &end, float k)
{
    float posX = start.transVec3.x + (end.transVec3.x - start.transVec3.x) * k;
    float posY = start.transVec3.y + (end.transVec3.y - start.transVec3.y) * k;
    float posZ = start.transVec3.z + (end.transVec3.z - start.transVec3.z) * k;

    float scaleX = start.scaleVec3.x + (end.scaleVec3.x - start.scaleVec3.x) * k;
    float scaleY = start.scaleVec3.y + (end.scaleVec3.y - start.scaleVec3.y) * k;
    float scaleZ = start.scaleVec3.z + (end.scaleVec3.z - start.scaleVec3.z) * k;

    float angle = start.angle + (end.angle - start.angle) * k;

    glm::vec3 axis = start.axis + (end.axis - start.axis) * k;

    return Transform(posX, posY, posZ, scaleX, scaleY, scaleZ, angle, axis);
}
