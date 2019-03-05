// Posture.h
// Author: Bairuo

#ifndef POSTURE_H_
#define POSTURE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Posture
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

    Posture(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ,
            float angle = 0, glm::vec3 axis = glm::vec3(0, 0, 1.0f));
    Posture(float x, float y, float z, float s);
    Posture(float x, float y, float z, glm::vec3 scaleVec3);
    Posture(glm::vec3 transVec3, float s);
    Posture(glm::vec3 transVec3, glm::vec3 scaleVec3);
    Posture(glm::vec3 transVec3, glm::vec3 scaleVec3, float angle, glm::vec3 axis);
    Posture();

    static Posture Lerp(const Posture &start, const Posture &end, float k);

private:
    glm::mat4 transMatrix;
    glm::mat4 scaleMatrix;
    glm::mat4 rotateMatrix;
};

#endif // POSTURE_H_
