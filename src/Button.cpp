#include "Button.h"
#include "BasicConfig.h"

#if defined(_WIN32)
const GLchar Button::standardVsPath[] = ".\\res\\standard.vs";
const GLchar Button::standardFragPath[] = ".\\res\\rounded.frag";
#else
const GLchar Button::standardVsPath[] = "./res/standard.vs";
const GLchar Button::standardFragPath[] = "./res/rounded.frag";
#endif

Button::Button(const glm::vec3 &pos, const Color &color, const Color &roundColor,
              float radius, float width, float height,
              const GLchar *vertexPath, const GLchar *fragmentPath)
              :Rectangle(pos, color, roundColor, radius, width, height, vertexPath, fragmentPath)
{

}

void Button::CoordinateTrans(double &x, double &y)
{
    y = -y;

    x = x / (WindowWidth / 2);
    y = y / (WindowWidth / 2);

    x = x - 1;
    y = y - (-1);
}

bool Button::isTrigger(double x, double y)
{
    Button::CoordinateTrans(x, y);

    if(x < transform().getPosX() + transform().getScaleX() / 2 && x > transform().getPosX() - transform().getScaleX() / 2)
    {
        if(y < transform().getPosY() + transform().getScaleY() / 2 && y > transform().getPosY() - transform().getScaleY() / 2)
        {
            return true;
        }
    }

    return false;
}
