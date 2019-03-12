#ifndef LIGHT_H
#define LIGHT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

class Transform;

extern Shader depthShader;

class Light
{
public:
    static const GLuint SHADOW_WIDTH;
    static const GLuint SHADOW_HEIGHT;

    static const GLfloat near_plane;
    static const GLfloat far_plane;

    static GLuint depthMapFBO;
    static GLuint depthMap;

    static bool depthMode;

    static void setLight(Shader *shader);

    static void openShadowMap();
};

#endif
