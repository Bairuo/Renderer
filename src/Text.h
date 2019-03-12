// Text.h
// Author: Bairuo

#ifndef TEXT_H_
#define TEXT_H_

#include<string>
#include <boost/shared_ptr.hpp>
#include "TextRenderer.h"
#include "Transform.h"
#include "property.h"
#include "Color.h"

class Animation;

class Text
{
public:
    Transform transform;
    Color color;
    int size;

    property_rw<std::string> text = property_rw<std::string>(
        property_set(std::string)
        {
            _text = value;
            shift.x = textRenderer->GetTexLength(text, size, true) / 2;
            shift.x = (int)(shift.x * 10000) * 1.0f / 10000;

            shift.y = textRenderer->GetTexHeight(text, size, true) * 0.74f / 2;
        },
        property_get(std::string)
        {
            return _text;
        }
    );

    boost::shared_ptr<Animation> animation;

    void Update();

    ~Text();

    Text(const std::string &text, const glm::vec3 &pos, const Color &color, int size);

private:
    TextRenderer *textRenderer;
    glm::vec2 shift;

    std::string _text;
};

#endif // TEXT_H_
