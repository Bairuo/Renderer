// Rectangle.h
// Author: Bairuo

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

#include "Color.h"
#include "Shader.h"
#include "Posture.h"
#include "Animation.h"
#include "property.h"

class Rectangle
{
public:
    float radius;
    boost::shared_ptr<Animation> animation;

    property_rw<Posture> posture = property_rw<Posture>(
        property_set(Posture)
        {
            postureChange = true;
            _posture = value;
        },
        property_get(Posture)
        {
            return _posture;
        }
    );

    property_rw<Color> color = property_rw<Color>(
        property_set(Color)
        {
            colorChange = true;
            _color = value;
        },
        property_get(Color)
        {
            return _color;
        }
    );

    virtual void Update();
    Rectangle();
    virtual ~Rectangle();

    Rectangle(const glm::vec3 &pos, const Color &color, const Color &roundColor,
              float radius, float scaleX, float scaleY,
              const GLchar *vertexPath = standardVsPath, const GLchar *fragmentPath = standardFragPath);

    void Init(const GLchar *vertexPath, const GLchar *fragmentPath, const Color &roundColor);


private:
    GLuint VAO;

    Shader shader;

    Color _color;
    Posture _posture;
    glm::vec3 _pos;

    bool active = true;
    bool colorChange = false;
    bool postureChange = false;

    static const GLchar standardVsPath[];
    static const GLchar standardFragPath[];
};


#endif // RECTANGLE_H_
