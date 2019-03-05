#include "Frame.h"

Frame::Frame() = default;

Frame::Frame(float time, const Posture &posture, const Color &color)
{
    this->time = time;
    this->posture = posture;
    this->color = color;
}
