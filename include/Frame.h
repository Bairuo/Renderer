// Frame.h
// Author: Bairuo

#ifndef FRAME_H_
#define FRAME_H_

#include "Color.h"
#include "Transform.h"

class Frame
{
public:
    float time;
    Transform transform;
    Color color;

    Frame();
    Frame(float time, const Transform &transform, const Color &color);
};

#endif // FRAME_H_
