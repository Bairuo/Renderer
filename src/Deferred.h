#ifndef DEFERRED_H_
#define DEFERRED_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

class Posture;

extern bool GBufferMode;

extern Shader deferredShader;

GLuint getGBuffer();

#endif
