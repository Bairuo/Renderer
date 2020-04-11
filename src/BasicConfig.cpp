#include "BasicConfig.h"
#include "Cube.h"
#include "IcoSphere.h"

GLFWwindow *window;
char WindowName[] = "Renderer";
extern const GLuint WindowWidth = 800;
extern const GLuint WindowHeight = 800;

extern const float kCardRadius = 0.01f;
extern const float kRefreshTime = 0.3f;
extern const float kMergeBornTime = 0.2f;

extern const Color kTitleColor(0.464f, 0.43f, 0.395f, 1.0f);
extern const Color kFPSColor(1, 0, 0, 1.0f);

#ifndef PBRTEST
extern const Color kBackColor(0.98f, 0.972f, 0.937f, 1.0f);
#else
extern const Color kBackColor(0, 0, 0, 1.0f);
#endif // !PBRTEST


#if defined(_WIN32)
extern const char ttfFile[] = ".\\res\\Arial.ttf";
#else
extern const char ttfFile[] = "./res/Arial.ttf";
#endif
