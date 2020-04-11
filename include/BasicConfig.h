// ConfigUtilities.h
// Author: Bairuo

#ifndef BASICCONFIG_H_
#define BASICCONFIG_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define PBRTEST

#ifdef PBRTEST
#define CAMERAROTATE
#endif // PBRTEST


#ifndef PBRTEST
#define SHADOWMAP
#define DEFERRED
#endif // !PBRTEST

class Color;

extern GLFWwindow *window;
extern char WindowName[];
extern const GLuint WindowWidth;
extern const GLuint WindowHeight;

extern const float kCardRadius;
extern const float kRefreshTime;
extern const float kMergeBornTime;

extern const Color kTitleColor;
extern const Color kFPSColor;
extern const Color kBackColor;

extern const char ttfFile[];

#endif // BASICCONFIG_H_
