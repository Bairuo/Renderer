#include "Rectangle.h"
#include "ConfigUtilities.h"
#include "basicrender.h"
#include "Camera.h"

#if defined(_WIN32)
const GLchar Rectangle::standardVsPath[] = ".\\shaders\\2D_Standard\\standard.vs";
const GLchar Rectangle::standardFragPath[] = ".\\shaders\\2D_Extend\\rounded.frag";
#else
const GLchar Rectangle::standardVsPath[] = "./shaders/2D_Standard/standard.vs";
const GLchar Rectangle::standardFragPath[] = "./shaders/2D_Extend/rounded.frag";
#endif

void Rectangle::Update()
{
    if(!active)
    {
        return;
    }

    shader.Use();

    if(animation.get() != nullptr)
    {
        color = animation->getColor();
        posture = animation->getPosture();

        if(animation->getActive() == false)
        {
            animation.reset();
        }
        else
        {
            animation->Update();
        }
    }

    if(colorChange)
    {
        shader.Set4f("setcolor", color().r, color().g, color().b, color().a);   // seems can not do like c#
        colorChange = false;
    }

    if(postureChange)
    {
        shader.SetMat4("model", posture().getMatrix());
        shader.SetFloat("recWidth", posture().getScaleX());
        shader.SetFloat("recHeight", posture().getScaleY());
        postureChange = false;
    }

    shader.SetMat4("projection", Camera::MainCamera->GetPersMatrix());
    shader.SetMat4("view", Camera::MainCamera->GetViewMatrix());

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader.Stop();
}

Rectangle::Rectangle() = default;

Rectangle::~Rectangle()
{
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
}

Rectangle::Rectangle(const glm::vec3 &pos, const Color &color, const Color &roundColor,
          float radius, float scaleX, float scaleY,
          const GLchar *vertexPath, const GLchar *fragmentPath)
          :radius(radius)
{
    posture = Posture(pos.x, pos.y, pos.z, scaleX, scaleY, 1);

    this->color = color;

    Init(vertexPath, fragmentPath, roundColor);
}

void Rectangle::Init(const GLchar *vertexPath, const GLchar *fragmentPath, const Color &roundColor)
{
    shader = Shader(vertexPath, fragmentPath);
    shader.Use();

    shader.SetInt("windowWidth", WindowWidth);
    shader.SetInt("windowHeight", WindowHeight);
    shader.Set4f("roundColor", roundColor.r, roundColor.g, roundColor.b, roundColor.a);
    shader.Set4f("setcolor", color().r, color().g, color().b, color().a);

    shader.SetFloat("radius", radius);

    shader.SetFloat("recWidth", posture().getScaleX());
    shader.SetFloat("recHeight", posture().getScaleX());

    shader.SetMat4("model", posture().getMatrix());

    VAO = getSquareVAO();

    shader.Stop();
}
