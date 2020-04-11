#include "Deferred.h"
#include "BasicConfig.h"
#include "Camera.h"
#include "Light.h"
#include <iostream>

#if defined(_WIN32)
const GLchar deferredGeometryVSPath[] = ".\\shaders\\3D_Standard\\deferred_geometry.vs";
const GLchar deferredGeometryFragPath[] = ".\\shaders\\3D_Standard\\deferred_geometry.frag";
const GLchar deferredLightingVSPath[] = ".\\shaders\\3D_Standard\\deferred_lighting.vs";
const GLchar deferredLightingFragPath[] = ".\\shaders\\3D_Standard\\deferred_lighting.frag";
#else
const GLchar deferredGeometryVSPath[] = "./shaders/3D_Standard/deferred_geometry.vs";
const GLchar deferredGeometryFragPath[] = "./shaders/3D_Standard/deferred_geometry.frag";
const GLchar deferredLightingVSPath[] = "./shaders/3D_Standard/deferred_lighting.vs";
const GLchar deferredLightingFragPath[] = "./shaders/3D_Standard/deferred_lighting.frag";
#endif

bool GBufferMode = false;

Shader deferredGeometryShader;
Shader deferredLightingsShader;

GLuint gPosition;
GLuint gPosLightSpace;
GLuint gNormal;

GLuint gAmbient;
GLuint gDiffuse;
GLuint gAlbedoSpec;

void deferredLightingRender()
{
    static GLuint quadVAO = 0;
    GLuint quadVBO;

    deferredLightingsShader.Use();

    Light::setLight(&deferredLightingsShader);

    Camera::setMainCamera(&deferredLightingsShader);

    if(quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    deferredLightingsShader.Stop();
}

GLuint getGBuffer()
{
    GLuint gBuffer;

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // - Position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
    // It seems that this can not work, before finding a way to use RGB buffer, using RGBA instead
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // - PosLightSpace color buffer
    glGenTextures(1, &gPosLightSpace);
    glBindTexture(GL_TEXTURE_2D, gPosLightSpace);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gPosLightSpace, 0);

    // - Normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gNormal, 0);

    // - Ambient color buffer
    glGenTextures(1, &gAmbient);
    glBindTexture(GL_TEXTURE_2D, gAmbient);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gAmbient, 0);

    // - Diffuse color buffer
    glGenTextures(1, &gDiffuse);
    glBindTexture(GL_TEXTURE_2D, gDiffuse);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gDiffuse, 0);

    // - Albedo + Specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, gAlbedoSpec, 0);


    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    GLuint attachments[6] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
                             GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5};
    glDrawBuffers(6, attachments);

    // - Create and attach depth buffer (renderbuffer)
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Deferred: Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    deferredGeometryShader = Shader(deferredGeometryVSPath, deferredGeometryFragPath);
    //deferredGeometryShader = Shader(".\\shaders\\3D_Standard\\standard_shadow.vs", ".\\shaders\\3D_Standard\\standard_shadow.frag");
    deferredLightingsShader = Shader(deferredLightingVSPath, deferredLightingFragPath);

    deferredLightingsShader.Use();

    deferredLightingsShader.SetInt("gPosition", 0);
    deferredLightingsShader.SetInt("gPosLightSpace", 1);
    deferredLightingsShader.SetInt("gNormal", 2);

    deferredLightingsShader.SetInt("gAmbient", 3);
    deferredLightingsShader.SetInt("gDiffuse", 4);
    deferredLightingsShader.SetInt("gAlbedoSpec", 5);

#if defined(SHADOWMAP)
    deferredLightingsShader.SetInt("shadowMap", 6);
#endif // defined

    deferredLightingsShader.Stop();

    return gBuffer;
}
