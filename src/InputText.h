// InputText.h
// Author: Bairuo

#ifndef INPUTTEXT_H_
#define INPUTTEXT_H_

#include <boost/shared_ptr.hpp>

#include "TextRenderer.h"
#include "Rectangle.h"
#include "Posture.h"
#include "property.h"

class Animation;

class InputText
{
public:
    Posture posture;
    Color color;
    int size;
    boost::shared_ptr<Rectangle> cursor;

    property_rw<std::string> text = property_rw<std::string>(
        property_set(std::string)
        {
            _text = value;
            shift.x = (textRenderer->GetTexLength(_text, size, true) + cursorWight) / 2;
            shift.x = (int)(shift.x * 10000) * 1.0f / 10000;

            shift.y = textRenderer->GetTexHeight(_text, size, true) * 0.74f / 2;

            glm::vec3 cursorPos = glm::vec3(posture.getPosX() + shift.x - cursorWight / 2, posture.getPosY() - cursorShift, posture.getPosZ());

            if(cursor != nullptr)
            {
                cursor->posture = Posture(cursorPos, cursor->posture().scaleVec3);

                if(cursor->animation != nullptr)
                {
                    cursor->animation->moveAllFramesTo(cursor->posture);
                }
            }

        },
        property_get(std::string)
        {
            return _text;
        }
    );

    void Update();

    ~InputText();

    InputText(const std::string &text, const glm::vec3 &pos, const Color &color, int size);

    void generateCurosAnimation(const Color &color, float time);

private:
    TextRenderer *textRenderer;
    glm::vec2 shift;
    float cursorShift = 0;

    float cursorWight = 0;
    float cursorHeight;

    std::string _text;
};

#endif // INPUTTEXT_H_
