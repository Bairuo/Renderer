#include "Text.h"
#include "Animation.h"

Text::~Text()
{

}

Text::Text(const std::string &text, const glm::vec3 &pos, const Color &color, int size)
    :posture(pos, 1),
     color(color),
     size(size),
     textRenderer(TextRenderer::GetInstance())
{
    this->text = text;
}

void Text::Update()
{
    if(animation.get() != nullptr)
    {
        color = animation->getColor();
        posture = animation->getPosture();

        if(animation->getActive() == false)
        {
            animation.reset();
        }
        else
        {
            animation->Update();
        }
    }

    textRenderer->DrawText(text, posture.getPosX() - shift.x, posture.getPosY() - shift.y, color, size, true);
}
