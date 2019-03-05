#include <iostream>
#include "Animation.h"
#include "GameTime.h"

Color Animation::getColor() const
{
    if(timer < 0)
    {
        return Color(0, 0, 0, 0);
    }

    if(pointer < frames.size() - 1)
    {
        float k = (timer - frames[pointer].time) / (frames[pointer + 1].time - frames[pointer].time);
        return Color::Lerp(frames[pointer].color, frames[pointer + 1].color, k);
    }
    else
    {
        return frames[pointer].color;
    }
}

Posture Animation::getPosture() const
{
    if(timer < 0)
    {
        return Posture(frames[pointer].posture.transVec3, 0);
    }

    if(pointer < frames.size() - 1)
    {
        float k = (timer - frames[pointer].time) / (frames[pointer + 1].time - frames[pointer].time);
        return Posture::Lerp(frames[pointer].posture, frames[pointer + 1].posture, k);
    }
    else
    {
        return frames[pointer].posture;
    }

}

Color Animation::getEndColor() const
{
    if(frames.size() > 0)
        return frames[frames.size() - 1].color;
    else
        return Color();
}

Posture Animation::getEndPosture() const
{
    if(frames.size() > 0)
        return frames[frames.size() - 1].posture;
    else
        return Posture();
}

void Animation::Start()
{
    active = true;
    pointer = 0;
    setTimer(startTime);
}

void Animation::Update()
{
    if(pause)
    {
        return;
    }

    timer += GameTime::deltaTime;

    while(pointer < frames.size() - 1 && timer > frames[pointer + 1].time)
    {
        pointer++;
    }

    if(pointer == frames.size() - 1)
    {
        if(loop)
        {
            timer = 0;
            pointer = 0;
        }
        else
        {
            active = false;
        }

    }
}

void Animation::setStartTime(float t)
{
    startTime = -t;
}

float Animation::getTimer() const
{
    return timer;
}

void Animation::setTimer(float t)
{
    timer = t;

    for(pointer = 0; pointer < frames.size() - 1;)
    {
        if(frames[pointer + 1].time < timer)
        {
            pointer++;
        }
        else
        {
            break;
        }
    }
}

void Animation::addFrame(float time, const Posture &posture)
{
    addFrame(time, posture, Color());
}

void Animation::addFrame(float time, const Posture &posture, const Color &color)
{
    frames.push_back(Frame(time, posture, color));
}

void Animation::moveAllFramesTo(const Posture &posture)
{
    for(unsigned int i = 0; i < frames.size(); i++)
    {
        frames[i].posture = posture;
    }
}

bool Animation::getActive() const
{
    return active;
}

//void Animation::setActive(bool active)
//{
//    this->active = active;
//}

bool Animation::getPause() const
{
    return pause;
}

void Animation::setPause(bool pause)
{
    this->pause = pause;
}

bool Animation::getLoop() const
{
    return loop;
}

void Animation::setLoop(bool loop)
{
    this->loop = loop;
}
