#include "InputText.h"
#include "Rectangle.h"

InputText::~InputText()
{

}

InputText::InputText(const std::string &text, const glm::vec3 &pos, const Color &color, int size)
    :transform(pos, 1),
     color(color),
     size(size)
{
    textRenderer = TextRenderer::GetInstance();

    cursorWight = textRenderer->GetCharWidth('a', size, true);
    cursorHeight = cursorWight * 0.31f;

    cursorShift = textRenderer->GetCharAdvy('a', size, true) * 0.74f / 2 * 1.25f;

    cursor.reset(new Rectangle(glm::vec3(0, 0, 0), Color(), Color(), 0, cursorWight, cursorHeight));

    this->text = text;
}

void InputText::generateCurosAnimation(const Color &color, float time)
{
    boost::shared_ptr<Animation> cursorAnimation(new Animation());
    cursorAnimation->setStartTime(time);
    cursorAnimation->setLoop(true);
    cursorAnimation->addFrame(0, cursor->transform(), Color(color.r, color.g, color.b, 1));
    cursorAnimation->addFrame(0.6f, cursor->transform(), Color(color.r, color.g, color.b, 1));
    cursorAnimation->addFrame(0.6f, cursor->transform(), Color(color.r, color.g, color.b, 0));
    cursorAnimation->addFrame(1.2f, cursor->transform(), Color(color.r, color.g, color.b, 0));
    cursorAnimation->start();

    cursor->animation = cursorAnimation;
}

void InputText::update()
{
    if(cursor.get() != nullptr)
    {
        cursor->update();
    }

    textRenderer->DrawText(text, transform.getPosX() - shift.x, transform.getPosY() - shift.y, color, size, true);

}
