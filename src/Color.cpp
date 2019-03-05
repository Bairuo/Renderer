#include "Color.h"

Color::Color():r(0), g(0), b(0), a(0) {}

Color::Color(float red, float green, float blue, float alpha):r(red), g(green), b(blue), a(alpha) {}

Color Color::Lerp(const Color &start, const Color &end, float k)
{
    float r = start.r + (end.r - start.r) * k;
    float g = start.g + (end.g - start.g) * k;
    float b = start.b + (end.b - start.b) * k;
    float a = start.a + (end.a - start.a) * k;

    return Color(r, g, b, a);
}
