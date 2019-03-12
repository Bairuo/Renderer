#ifndef RIGIDBODY_H
#define RIGIDBODY_H

class Object;

class RigidBody
{
public:
    Object *obj = nullptr;

    virtual void Update() = 0;

    virtual ~RigidBody() {}
};

#endif // RIGIDBODY_H
