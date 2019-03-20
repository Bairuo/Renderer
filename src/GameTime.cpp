#include "GameTime.h"
#include <cstdio>
#include <iostream>
#include <GLFW/glfw3.h>

float GameTime::deltaTime = 0;
GameTime* GameTime::instance = nullptr;

GameTime* GameTime::GetInstance()
{
    if(instance == nullptr)
    {
        instance = new GameTime();
    }
    return instance;
}

void GameTime::Init()
{
    //lastTime = clock();
    lastTime = glfwGetTime();
}

int GameTime::GetFPS()
{
    static float delta = 0;
    static int lastfps = 0;
    static int deltaNum = 0;

    delta += deltaTime;
    deltaNum++;

    if(delta > 1)
    {
        lastfps = deltaNum;

        delta = 0;
        deltaNum = 0;

        return lastfps;
    }
    else
    {
        return lastfps;
    }
}

void GameTime::update()
{
    float curTime = glfwGetTime();
    deltaTime = curTime - lastTime;
    lastTime = curTime;
}
