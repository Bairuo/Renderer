#include "AlignedText.h"
#include "TextRenderer.h"
#include <string>
#include <iostream>
#include <sstream>
#include "ConfigUtilities.h"

static const float boundary = 0.5f;

static const Color titleColor(0.98f, 0.972f, 0.937f, 1.0f);
static const Color valueColor(1.0f, 1.0f, 1.0f, 1.0f);

#if defined(_WIN32)
const GLchar AlignedText::standardVsPath[] = ".\\res\\standard.vs";
const GLchar AlignedText::standardFragPath[] = ".\\res\\rounded.frag";
#else
const GLchar AlignedText::standardVsPath[] = "./res/standard.vs";
const GLchar AlignedText::standardFragPath[] = "./res/rounded.frag";
#endif

void AlignedText::SetValue(int value)
{
    std::ostringstream os;
    os << value;
    this->value = os.str();
}

AlignedText::AlignedText(const std::string &title, int value, float radius,
                        const glm::vec3 &pos, const Color &color, const Color &roundColor, float width, float height,
                        const GLchar *vertexPath, const GLchar *fragmentPath
                        ):Rectangle(pos, color, roundColor, radius, width, height, vertexPath, fragmentPath)
{
    this->title = title;
    SetValue(value);

    this->valueHeight = pos.y - height / 2; this->titleHeight = pos.y - height / 2 + height * boundary;
    float valueSize = height * boundary * 0.8f, titleSize = height * (1 - boundary) * 0.6f;

    this->valueHeight += (height * boundary) * (1 - valueSize) * 0.35f;
    this->titleHeight += (height * (1 - boundary)) * (1 - titleSize) * 0.35f;

    this->valueSize = valueSize * WindowWidth / 2;
    this->titleSize = titleSize * WindowWidth / 2;


    this->valueBold = true;
    this->titleBold = false;

    textRenderer = TextRenderer::GetInstance();
}


void AlignedText::Update()
{
    Rectangle::Update();

    textRenderer->DrawText(title, posture().getPosX() - textRenderer->GetTexLength(title, titleSize, titleBold) / 2, titleHeight, titleColor, titleSize, titleBold);
    textRenderer->DrawText(value, posture().getPosX() - textRenderer->GetTexLength(value, valueSize, valueBold) / 2, valueHeight, valueColor, valueSize, valueBold);
}
