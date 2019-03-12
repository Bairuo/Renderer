#include "Object.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Transform.h"
#include "Shader.h"

int ObjectID = 0;

std::vector<boost::shared_ptr<Object> > Objects;

Object::Object(int id, Transform *transform, Renderer *renderer, RigidBody *rigidbody, Animation *animation)
    :id(id)
{
    this->transform.reset(transform);
    this->renderer.reset(renderer);
    this->rigidbody.reset(rigidbody);
    this->animation.reset(animation);
}

Object::~Object()
{

}

void Object::Update()
{
    if(rigidbody.get() != nullptr)
    {
        rigidbody->Update();
    }

    if(animation.get() != nullptr)
    {
        if(animation->getActive() == false)
        {
            animation.reset();
        }
        else
        {
            animation->Update();
        }

        dirty = true;
    }

    if(transform.get() != nullptr)
    {
        transform->Update();
    }

    // other components
    // ...
}

void Object::Render(Shader &shader)
{
    if(renderer.get() != nullptr)
    {
        renderer->Render(shader);
    }
}

void Object::Render()
{
    if(renderer.get() != nullptr)
    {
        renderer->Render();
    }
}

boost::shared_ptr<Object> generateObject(Transform *transform, Renderer *renderer, RigidBody *rigidbody, Animation *animation)
{
    boost::shared_ptr<Object> newObj(new Object(ObjectID, transform, renderer, rigidbody, animation));

    if(transform != nullptr)
    {
        newObj->transform->obj = newObj.get();
    }

    if(renderer != nullptr)
    {
        newObj->renderer->obj = newObj.get();
    }

    if(rigidbody != nullptr)
    {
        newObj->rigidbody->obj = newObj.get();
    }

    Objects.push_back(
        newObj
    );

    return newObj;
}
