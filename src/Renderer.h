#ifndef RENDERER_H
#define RENDERER_H

class Object;
class Shader;

class Renderer
{
public:
    Object *obj = nullptr;

    virtual void Render() = 0;

    virtual void Render(Shader &shader) = 0;

    virtual ~Renderer() {}

    virtual double getSphereBoundingRadius();
};

#endif // RENDERER_H
