#include <iostream>
#include <boost/shared_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "TextRenderer.h"
#include "bairuofunctions.h"
#include "GameTime.h"
#include "KeyUtilities.h"
#include "BasicConfig.h"
#include "Cuboid.h"
#include "IcoSphere.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "SceneConfig.h"
#include "Transform.h"
#include "Animation.h"
#include "BVH.h"
#include "Deferred.h"
#include "GraphNode.h"
#include "SceneConfig.h"
#include "Color.h"
#include "PBRMaterial.h"
#include <list>

#if defined(PBRTEST)
#define OBJNUM 2
static const glm::vec3 kObjPosConfig[OBJNUM] = {
	glm::vec3(-0.5f, -0.5f, 0),
	glm::vec3(0.5f, -0.5f, 0)
};

static const glm::vec3 kObjScaleConfig[OBJNUM] = {
	glm::vec3(0.35f, 0.35f, 0.35f),
	glm::vec3(0.35f, 0.35f, 0.35f)
};

static const boost::shared_ptr<Material> exampleMaterials[OBJNUM]{
	boost::shared_ptr<Material>(new PBRMaterial(
		glm::vec3(1.0f, 0, 0),	// albedo
		0.03f,	// metallic
		0.2f,	// roughness
		0.1f	// ao
	)),
	boost::shared_ptr<Material>(new PBRMaterial(
		glm::vec3(1.0f, 0, 0),	// albedo
		0.03f,	// metallic
		0.4f,	// roughness
		0.1f	// ao
	))
};

#else
#define OBJNUM 8
static const glm::vec3 kObjPosConfig[OBJNUM] = {
	glm::vec3(0, -0.4f, 1.0f),
	glm::vec3(-0.4f, 0, 1.0f),
	glm::vec3(0.4f, 0, 1.0f),
	glm::vec3(0, 0.4f, 1.0f),
	glm::vec3(0, 0.4f, -1.0f),
	glm::vec3(0.4f, 0, -1.0f),
	glm::vec3(-0.4f, 0, -1.0f),
	glm::vec3(0, -0.4f, -1.0f)
};

static const glm::vec3 kObjScaleConfig[OBJNUM] = {
	glm::vec3(0.35f, 0.35f, 0.35f),
	glm::vec3(0.35f, 0.6f, 0.35f),
	glm::vec3(0.35f, 0.35f, 0.35f),
	glm::vec3(0.35f, 0.35f, 0.35f),
	glm::vec3(0.35f, 0.35f, 0.35f),
	glm::vec3(0.35f, 0.6f, 0.35f),
	glm::vec3(0.35f, 0.35f, 0.35f),
	glm::vec3(0.35f, 0.35f, 0.35f)
};
#endif

static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float lastX = WindowWidth / 2.0f;
static float lastY = WindowHeight / 2.0f;
static bool firstMouse = true;


static GameTime mainTime;

// for contact test
static PotentialContact potentialContacts[MAXDETECTNUM];

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
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
#if defined(PBRTEST)
		auto obj = generateObject(new Transform(kObjPosConfig[i], kObjScaleConfig[i], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f)),
			new IcoSphere(exampleMaterials[i]));
#else
		auto obj = generateObject(new Transform(kObjPosConfig[i], kObjScaleConfig[i], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f)),
			new Cuboid());
#endif

		BVHTreeBuild(BVHTree, obj);
    }
#ifndef PBRTEST
	// Animation Demo set
	Transform start(kObjPosConfig[3], kObjScaleConfig[3], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));
	Transform end(glm::vec3(0, 0.4f, 0), kObjScaleConfig[3], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));

	Objects[3]->animation.reset(new Animation());
	Objects[3]->animation->addFrame(0, start);
	Objects[3]->animation->addFrame(3.0f, end);
	Objects[3]->animation->addFrame(6.0f, start);
	Objects[3]->animation->setLoop(true);
	Objects[3]->animation->start();

	// Sub object example
	//Objects[3]->addSubObject(generateObject(new Transform(glm::vec3(0, 0.8f, 0), glm::vec3(1)), new Cuboid()));

	start = Transform(kObjPosConfig[5], kObjScaleConfig[5], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));
	end = Transform(glm::vec3(0.4f, 0, 0), kObjScaleConfig[5], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));

	Objects[5]->animation.reset(new Animation());
	Objects[5]->animation->addFrame(0, start);
	Objects[5]->animation->addFrame(3.0f, end);
	Objects[5]->animation->addFrame(6.0f, start);
	Objects[5]->animation->setLoop(true);
	Objects[5]->animation->start();
#endif // !PBRTEST

    /* rendering setting */
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(kBackColor.r, kBackColor.g, kBackColor.b, kBackColor.a);

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
        processInput(window, camera);

        // Event
        glfwPollEvents();

        // Game update
        SceneGraph.update(glm::mat4(1), false);

        unsigned potentialDebug;

        if(BVHTree != nullptr)
        {
            // BVH update
            BVHTree->update(BVHTree);

            potentialDebug = BVHTree->getPotentialContacts(potentialContacts);
        }
        else
        {
            potentialDebug = 0;
        }

        // render
        glEnable(GL_DEPTH_TEST);

#if defined(DEFERRED)
        GBufferMode = true;
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(auto iter = Objects.begin(); iter != Objects.end(); ++iter)
        {
            iter->second->render(deferredGeometryShader);
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

        for(auto iter = Objects.begin(); iter != Objects.end(); ++iter)
        {
            iter->second->render(depthShader);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Recovery Setting
        glViewport(0, 0, WindowWidth, WindowHeight);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Light::depthMode = false;

#endif

        // Normal render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if defined(DEFERRED)

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gPosLightSpace);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gAmbient);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, gDiffuse);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

#if defined(SHADOWMAP)
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, Light::depthMap);
#endif

        deferredLightingRender();

#else

#if defined(SHADOWMAP)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Light::depthMap);
#endif

        for(size_t i = 0; i < Objects.size(); i++)
        {
            Objects[i]->render();
        }

#endif // defined DEFERRED

        glDisable(GL_DEPTH_TEST);

#ifndef PBRTEST
		textRenderer->DrawText("Fun Renderer", -384, 358, kTitleColor, 32, true);
		textRenderer->DrawText("FPS:" + bairuo::int2str(GameTime::GetFPS()), -384, 326, kFPSColor, 16, true);
		textRenderer->DrawText("WASD to move ", -384, -334, kTitleColor, 16, true);
		textRenderer->DrawText("Potential contacts: " + bairuo::uns2str(potentialDebug), -384, -358, kTitleColor, 16, true);
#endif // !PBRTEST

        glfwSwapBuffers(window);

        mainTime.update();

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
#ifdef CAMERAROTATE
	glfwSetCursorPosCallback(window, mouse_callback);
#endif // CAMERAROTATE
    glfwSetScrollCallback(window, scroll_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
