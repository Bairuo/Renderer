#include "Frame.h"

Frame::Frame() = default;

Frame::Frame(float time, const Transform &transform, const Color &color)
{
    this->time = time;
    this->transform = transform;
    this->color = color;
}
