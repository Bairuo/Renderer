// GameTime.h
// Author: Bairuo

#ifndef GAMETIME_H_
#define GAMETIME_H_

class GameTime
{
public:
    static float deltaTime;

    void Init();
    void Update();

    static GameTime* GetInstance();
    static int GetFPS();

private:
    static GameTime* instance;
    float lastTime;

};

#endif // GAMETIME_H_
