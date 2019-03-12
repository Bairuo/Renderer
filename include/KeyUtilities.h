// KeyUtilities.h
// Author: Bairuo

#ifndef KEYUTILITIES_H_
#define KEYUTILITIES_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera;

char KeyToChar(bool capital, int key);
bool isCapsActive();

void processInput(GLFWwindow *window, Camera &camera);

void mouse_callback(GLFWwindow* window, Camera &camera, double xpos, double ypos);

#endif // KEYUTILITIES_H_
