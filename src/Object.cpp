#include "Object.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Posture.h"

int ObjectID = 0;

std::vector<boost::shared_ptr<Object> > Objects;

Object::Object(int id, Posture *posture, Renderer *renderer, RigidBody *rigidbody, Animation *animation)
    :id(id)
{
    this->posture.reset(posture);
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

    // other components
    // ...
}

void Object::Render()
{
    if(renderer.get() != nullptr)
    {
        renderer->Update();
    }
}

boost::shared_ptr<Object> generateObject(Posture *posture, Renderer *renderer, RigidBody *rigidbody, Animation *animation)
{
    boost::shared_ptr<Object> newObj(new Object(ObjectID, posture, renderer, rigidbody, animation));

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
