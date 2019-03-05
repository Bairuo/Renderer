// Color.h
// Author: Bairuo

#ifndef COLOR_H_
#define COLOR_H_

class Color
{
public:
    float r, g, b, a;

    Color();
    Color(float red, float green, float blue, float alpha);
    static Color Lerp(const Color &start, const Color &end, float k);
};


#endif // COLOR_H_
