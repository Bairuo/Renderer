#include "Rectangle.h"
#include "BasicConfig.h"
#include "basicrender.h"
#include "Camera.h"

#if defined(_WIN32)
const GLchar Rectangle::kStandardVsPath[] = ".\\shaders\\2D_Standard\\standard.vs";
const GLchar Rectangle::kStandardFragPath[] = ".\\shaders\\2D_Extend\\rounded.frag";
#else
const GLchar Rectangle::kStandardVsPath[] = "./shaders/2D_Standard/standard.vs";
const GLchar Rectangle::kStandardFragPath[] = "./shaders/2D_Extend/rounded.frag";
#endif

void Rectangle::update()
{
    if(!active)
    {
        return;
    }

    shader.Use();

    if(animation.get() != nullptr)
    {
        color = animation->getColor();
        transform = animation->getTransform();

        if(animation->getActive() == false)
        {
            animation.reset();
        }
        else
        {
            animation->update();
        }
    }

    if(colorChange)
    {
        shader.Set4f("setcolor", color().r, color().g, color().b, color().a);   // seems can not do like c#
        colorChange = false;
    }

    if(transformChange)
    {
        shader.SetMat4("model", transform().getMatrix());
        shader.SetFloat("recWidth", transform().getScaleX());
        shader.SetFloat("recHeight", transform().getScaleY());
        transformChange = false;
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
    transform = Transform(pos.x, pos.y, pos.z, scaleX, scaleY, 1);

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

    shader.SetFloat("recWidth", transform().getScaleX());
    shader.SetFloat("recHeight", transform().getScaleX());

    shader.SetMat4("model", transform().getMatrix());

    VAO = getSquareVAO();

    shader.Stop();
}
