#include "Object.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Transform.h"
#include "Shader.h"
#include "GraphNode.h"

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

void Object::update()
{
    if(rigidbody.get() != nullptr)
    {
        rigidbody->update();
    }

    if(animation.get() != nullptr)
    {
        if(animation->getActive() == false)
        {
            animation.reset();
        }
        else
        {
            animation->update();
        }

        dirty = true;
    }

    if(transform.get() != nullptr)
    {
        transform->update();
    }

    // other components
    // ...
}

void Object::render(Shader &shader)
{
    if(renderer.get() != nullptr)
    {
        renderer->render(shader);
    }
}

void Object::render()
{
    if(renderer.get() != nullptr)
    {
        renderer->render();
    }
}

void Object::addSubObject(boost::shared_ptr<Object> obj)
{
    if(graphNode != nullptr)
    {
        graphNode->addSubObject(obj.get());
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

    ObjectID++;

    return newObj;
}
