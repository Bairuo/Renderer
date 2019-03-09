#include <iostream>
#include <boost/shared_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "TextRenderer.h"
#include "bairuofunctions.h"
#include "GameTime.h"
#include "KeyUtilities.h"
#include "ConfigUtilities.h"
#include "Cuboid.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Posture.h"
#include "Animation.h"
#include "BVH.h"
#include "Deferred.h"

#include <list>

static const Color titleColor(0.464f, 0.43f, 0.395f, 1.0f);
static const Color FPSColor(1, 0, 0, 1.0f);
static const Color backGround(0.98f, 0.972f, 0.937f, 1.0f);

#if defined(_WIN32)
static const char ttfFile[] = ".\\res\\Arial.ttf";
#else
static const char ttfFile[] = "./res/Arial.ttf";
#endif

#define OBJNUM 8
static const glm::vec3 objPos[OBJNUM] = {
    glm::vec3(0, -0.4f, 1.0f),
    glm::vec3(-0.4f, 0, 1.0f),
    glm::vec3(0.4f, 0, 1.0f),
    glm::vec3(0, 0.4f, 1.0f),
    glm::vec3(0, 0.4f, -1.0f),
    glm::vec3(0.4f, 0, -1.0f),
    glm::vec3(-0.4f, 0, -1.0f),
    glm::vec3(0, -0.4f, -1.0f)
};

static const glm::vec3 objScale[OBJNUM] = {
    glm::vec3(0.35f, 0.35f, 0.35f),
    glm::vec3(0.35f, 0.6f, 0.35f),
    glm::vec3(0.35f, 0.35f, 0.35f),
    glm::vec3(0.35f, 0.35f, 0.35f),
    glm::vec3(0.35f, 0.35f, 0.35f),
    glm::vec3(0.35f, 0.6f, 0.35f),
    glm::vec3(0.35f, 0.35f, 0.35f),
    glm::vec3(0.35f, 0.35f, 0.35f)
};

static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

static GameTime mainTime;

static BVHNode<BoundingSphere> *BVHTree = nullptr;
static PotentialContact potentialContacts[MAXDETECTNUM];

static void processInput(GLFWwindow *window);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void InitWindow();

int main()
{
    InitWindow();

    /* subsystem start-up */
    TextRenderer* textRenderer = TextRenderer::GetInstance();
    textRenderer->Load(ttfFile);

    /* object generate */
    Camera::MainCamera = &camera;

    for(int i = 0; i < OBJNUM; i++)
    {
        auto obj = generateObject(new Posture(objPos[i], objScale[i], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f)),
                                  new Cuboid());

        // BVH Build
        if(obj->renderer.get() != nullptr)
        {
            /**
             * Default Contact test. Note even the object has not a rigidbody,
             * it can also being tested(like Trigger test).
             */
            double bRadius = obj->renderer->getSphereBoundingRadius();

            if(bRadius > 0)
            {
                double debug = 1;     // debug

                BoundingSphere bSphere(objPos[i], bRadius * debug);

                if(BVHTree == nullptr)
                {
                    BVHTree = new BVHNode<BoundingSphere>(NULL, bSphere, obj.get());
                }
                else
                {
                    BVHTree->insert(obj.get(), bSphere);
                }
            }
        }
    }

    // Animation Demo set
    Posture start(objPos[3], objScale[3], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));
    Posture end(glm::vec3(0, 0.4f, 0), objScale[3], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));

    Objects[3]->animation.reset(new Animation());
    Objects[3]->animation->addFrame(0, start);
    Objects[3]->animation->addFrame(3.0f, end);
    Objects[3]->animation->addFrame(6.0f, start);
    Objects[3]->animation->setLoop(true);
    Objects[3]->animation->Start();

    start = Posture(objPos[5], objScale[5], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));
    end = Posture(glm::vec3(0.4f, 0, 0), objScale[5], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));

    Objects[5]->animation.reset(new Animation());
    Objects[5]->animation->addFrame(0, start);
    Objects[5]->animation->addFrame(3.0f, end);
    Objects[5]->animation->addFrame(6.0f, start);
    Objects[5]->animation->setLoop(true);
    Objects[5]->animation->Start();

    /* rendering setting */
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if defined(DEFERRED)
    GLuint gBuffer = getGBuffer();
#endif // defined

#if defined(SHADOWMAP)
    Light::openShadowMap();
#endif // defined

    // Time
    mainTime.Init();

    /* Game loop */
    while(!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Event
        glfwPollEvents();

        // Game update
        for(size_t i = 0; i < Objects.size(); i++)
        {
            Objects[i]->Update();
        }

        unsigned potentialDebug;

        if(BVHTree != nullptr)
        {
            // BVH Update
            //std::cout << "before update" << std::endl;
            BVHTree->Update(BVHTree);

            //std::cout << "after update" << std::endl;

            potentialDebug = BVHTree->getPotentialContacts(potentialContacts);

            //std::cout << "after potential" << std::endl;
        }
        else
        {
            potentialDebug = 0;
        }

        // Render
        glEnable(GL_DEPTH_TEST);

#if defined(DEFERRED)
        GBufferMode = true;
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(size_t i = 0; i < Objects.size(); i++)
        {
            Objects[i]->Render();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GBufferMode = false;

#endif // defined

#if defined(SHADOWMAP)
        // render depth map
        Light::depthMode = true;
        glViewport(0, 0, Light::SHADOW_WIDTH, Light::SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, Light::depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        for(size_t i = 0; i < Objects.size(); i++)
        {
            Objects[i]->Render();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Recovery Setting
        glViewport(0, 0, WindowWidth, WindowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Light::depthMode = false;

#endif

        // Normal Render

        glClearColor(backGround.r, backGround.g, backGround.b, backGround.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if defined(DEFERRED)

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

#endif // defined


#if defined(SHADOWMAP)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Light::depthMap);
#endif

        for(size_t i = 0; i < Objects.size(); i++)
        {
            Objects[i]->Render();
        }

        glDisable(GL_DEPTH_TEST);

        textRenderer->DrawText("Fun Renderer", -384, 358, titleColor, 32, true);
        textRenderer->DrawText("FPS:" + bairuo::int2str(GameTime::GetFPS()), -384, 326, FPSColor, 16, true);
        textRenderer->DrawText("WASD to move ", -384, -334, titleColor, 16, true);
        textRenderer->DrawText("Potential contacts: " + bairuo::uns2str(potentialDebug), -384, -358, titleColor, 16, true);

        glfwSwapBuffers(window);

        mainTime.Update();

    }

    //delete BVHTree;

    glfwTerminate();

    return 0;
}


void InitWindow()
{
    glfwInit();

//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(WindowWidth, WindowHeight, WindowName, nullptr, nullptr);

    glfwMakeContextCurrent(window);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, GameTime::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, GameTime::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, GameTime::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, GameTime::deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static float lastX = WindowWidth / 2.0f;
    static float lastY = WindowHeight / 2.0f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
