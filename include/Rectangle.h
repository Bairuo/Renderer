// Rectangle.h
// Author: Bairuo

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

#include "Color.h"
#include "Shader.h"
#include "Transform.h"
#include "Animation.h"
#include "property.h"

class Rectangle
{
public:
    float radius;
    boost::shared_ptr<Animation> animation;

    property_rw<Transform> transform = property_rw<Transform>(
        property_set(Transform)
        {
            transformChange = true;
            _transform = value;
        },
        property_get(Transform)
        {
            return _transform;
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
    Transform _transform;
    glm::vec3 _pos;

    bool active = true;
    bool colorChange = false;
    bool transformChange = false;

    static const GLchar standardVsPath[];
    static const GLchar standardFragPath[];
};


#endif // RECTANGLE_H_
