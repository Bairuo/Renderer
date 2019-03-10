#ifndef DEFERRED_H_
#define DEFERRED_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

extern bool GBufferMode;

extern Shader deferredGeometryShader;
extern Shader deferredLightingsShader;

extern GLuint gPosition;
extern GLuint gPosLightSpace;

extern GLuint gNormal;
extern GLuint gAmbient;
extern GLuint gDiffuse;
extern GLuint gAlbedoSpec;

GLuint getGBuffer();
void deferredLightingRender();

#endif
