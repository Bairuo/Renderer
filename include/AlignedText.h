// AlignedText.h
// Author: Bairuo

#ifndef ALIGNEDTEXT_H_
#define ALIGNEDTEXT_H_

#include "Rectangle.h"
#include <string>

class TextRenderer;

class AlignedText : public Rectangle
{
public:
    std::string title;
    std::string value;

    void update();

    void SetValue(int value);

    AlignedText(const std::string &title, int value, float radius,
                const glm::vec3 &pos, const Color &color, const Color &roundColor, float width, float height,
                const GLchar *vertexPath = kStandardVsPath, const GLchar *fragmentPath = kStandardFragPath);

private:
    TextRenderer *textRenderer;

    float titleHeight;
    float valueHeight;
    int titleSize;
    int valueSize;
    bool titleBold;
    bool valueBold;

    static const GLchar kStandardVsPath[];
    static const GLchar kStandardFragPath[];
};

#endif // ALIGNEDTEXT_H_
