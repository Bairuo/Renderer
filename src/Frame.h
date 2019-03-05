// Frame.h
// Author: Bairuo

#ifndef FRAME_H_
#define FRAME_H_

#include "Color.h"
#include "Posture.h"

class Frame
{
public:
    float time;
    Posture posture;
    Color color;

    Frame();
    Frame(float time, const Posture &posture, const Color &color);
};

#endif // FRAME_H_
