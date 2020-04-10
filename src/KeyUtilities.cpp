#include "KeyUtilities.h"
#include "Camera.h"
#include "GameTime.h"
#include "BasicConfig.h"

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

void processInput(GLFWwindow *window, Camera &camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, GameTime::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, GameTime::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, GameTime::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, GameTime::deltaTime);
}

void mouse_callback(GLFWwindow* window, Camera &camera, double xpos, double ypos)
{
    static bool firstMouse = true;
    static float lastX = WindowWidth / 2.0f;
    static float lastY = WindowHeight / 2.0f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

