#include "KeyUtilities.h"

#if defined(_WIN32)
// No include "wingdi.h", name conflict
#define NOGDI
#include <windows.h>
// WINUSERAPI short __stdcall GetKeyState(int);
// Define these to solve name conflict
#define DrawTextA DrawText
#define DrawTextW DrawText
#else
#include <X11/XKBlib.h>
#endif

char KeyToChar(bool capital, int key)
{
    int shift;

    if(key >= 65 && key <= 90)
    {
        if(capital)
            shift = 0;
        else
            shift = 32;

        return key + shift;
    }
    else
    {
        if(key >= 48 && key <= 57)
        {
            shift = 0;
        }
        else if(key >= 320 && key <= 329)
        {
            shift = -272;
        }
        else
        {
            return 0;
        }

        return key + shift;
    }
}

bool isCapsActive()
{
    bool capital;
#if defined(_WIN32)
    if(GetKeyState(20) & 0x0001)    // Windows API, Uppercase is activated
    {
        capital = true;
    }
    else
    {
        capital = false;
    }
#else
    Display * d = XOpenDisplay((char*)0);

    if (d) {
        unsigned n;
        XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
        capital = (n & 1) ? true : false;
    }
#endif
    return capital;
}
