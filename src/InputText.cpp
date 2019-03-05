#include "InputText.h"
#include "Rectangle.h"

InputText::~InputText()
{

}

InputText::InputText(const std::string &text, const glm::vec3 &pos, const Color &color, int size)
    :posture(pos, 1),
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
    cursorAnimation->addFrame(0, cursor->posture(), Color(color.r, color.g, color.b, 1));
    cursorAnimation->addFrame(0.6f, cursor->posture(), Color(color.r, color.g, color.b, 1));
    cursorAnimation->addFrame(0.6f, cursor->posture(), Color(color.r, color.g, color.b, 0));
    cursorAnimation->addFrame(1.2f, cursor->posture(), Color(color.r, color.g, color.b, 0));
    cursorAnimation->Start();

    cursor->animation = cursorAnimation;
}

void InputText::Update()
{
    if(cursor.get() != nullptr)
    {
        cursor->Update();
    }

    textRenderer->DrawText(text, posture.getPosX() - shift.x, posture.getPosY() - shift.y, color, size, true);

}
