#include "BasicConfig.h"
#include "Cuboid.h"
#include "IcoSphere.h"

GLFWwindow *window;
char WindowName[] = "Renderer";
extern const GLuint WindowWidth = 800;
extern const GLuint WindowHeight = 800;

extern const float kCardRadius = 0.01f;
extern const float kRefreshTime = 0.3f;
extern const float kMergeBornTime = 0.2f;

#if defined(_WIN32)
extern const char ttfFile[] = ".\\res\\Arial.ttf";
#else
extern const char ttfFile[] = "./res/Arial.ttf";
#endif
