// ConfigUtilities.h
// Author: Bairuo

#ifndef BASICCONFIG_H_
#define BASICCONFIG_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SHADOWMAP
//#define PBRTEST

#ifndef PBRTEST
#define DEFERRED
#endif // !PBRTEST


extern GLFWwindow *window;
extern char WindowName[];
extern const GLuint WindowWidth;
extern const GLuint WindowHeight;

extern const float kCardRadius;
extern const float kRefreshTime;
extern const float kMergeBornTime;

extern const char ttfFile[];

#endif // BASICCONFIG_H_
