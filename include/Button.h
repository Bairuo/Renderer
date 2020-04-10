// Button.h
// Author: Bairuo

#ifndef BUTTON_H_
#define BUTTON_H_

#include "Rectangle.h"

class Button : public Rectangle
{
public:

    static void CoordinateTrans(double &x, double &y);

    bool isTrigger(double x, double y);

    Button(const glm::vec3 &pos, const Color &color, const Color &roundColor,
              float radius, float width, float height,
              const GLchar *vertexPath = kStandardVsPath, const GLchar *fragmentPath = kStandardFragPath);

private:
    static const GLchar kStandardVsPath[];
    static const GLchar kStandardFragPath[];
};

#endif // BUTTON_H_
