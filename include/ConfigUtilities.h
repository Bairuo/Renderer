// ConfigUtilities.h
// Author: Bairuo

#ifndef CONFIGUTILITIES_H_
#define CONFIGUTILITIES_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SHADOWMAP
#define DEFERRED

extern GLFWwindow *window;
extern char WindowName[];
extern const GLuint WindowWidth;
extern const GLuint WindowHeight;

extern const float c_cardRadius;
extern const float c_refreshTime;
extern const float c_mergeBornTime;

#endif // CONFIGUTILITIES_H_
