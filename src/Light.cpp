#include "Light.h"
#include "Posture.h"

#if defined(_WIN32)
const GLchar shadowmapVSPath[] = ".\\shaders\\3D_Standard\\shadowmap.vs";
const GLchar shadowmapFragPath[] = ".\\shaders\\3D_Standard\\shadowmap.frag";
#else
const GLchar shadowmapVSPath[] = "./shaders/3D_Standard/shadowmap.vs";
const GLchar shadowmapFragPath[] = "./shaders/3D_Standard/shadowmap.frag";
#endif

const GLuint Light::SHADOW_WIDTH = 4096;
const GLuint Light::SHADOW_HEIGHT = 4096;
const GLfloat Light::near_plane = 1.0f;
const GLfloat Light::far_plane = 7.5f;

Shader Light::depthShader;

GLuint Light::depthMapFBO;
GLuint Light::depthMap;
bool Light::depthMode = false;

static glm::vec3 pos1(-0.55f, 0, 1.0f);
static glm::vec3 pos(0.7f, 0.2f, 2.0f);

void Light::setLight(Shader *shader)
{
    shader->Set3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader->Set3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader->Set3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader->Set3f("dirLight.specular", 0.5f, 0.5f, 0.5f);

    shader->Set3f("pointLights[0].position", pos);
    shader->Set3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader->Set3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader->Set3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->SetFloat("pointLights[0].constant", 1.0f);
    shader->SetFloat("pointLights[0].linear", 0.09);
    shader->SetFloat("pointLights[0].quadratic", 0.032);

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;

    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    return;
}

void Light::openShadowMap()
{
    depthShader = Shader(shadowmapVSPath, shadowmapFragPath);

    // Configure depth map FBO
    glGenFramebuffers(1, &depthMapFBO);
    // - Create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::startRenderDepth(const Posture * const posture)
{
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;

    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;


    depthShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(depthShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(posture->getMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(depthShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    return;
}

void Light::stopRenderDepth()
{
    depthShader.Stop();

    return;
}
