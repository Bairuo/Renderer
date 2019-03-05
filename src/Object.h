#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <boost/shared_ptr.hpp>

class Posture;
class Renderer;
class RigidBody;
class Animation;

class Object
{
public:
    int id;
    bool dirty = false;     // If true, object need to be updated in Spatial Data Structure

    // Components
    // Can change to array
    // RigidBody *rigidbody[MAXSIZE]
    boost::shared_ptr<Posture> posture;     // History problem, may it need to be called 'Transform'
    boost::shared_ptr<Renderer> renderer;
    boost::shared_ptr<RigidBody> rigidbody;
    boost::shared_ptr<Animation> animation;

    Object(int id, Posture *posture, Renderer *renderer, RigidBody *rigidbody, Animation *animation);
    ~Object();

    void Update();
    void Render();

    // ...
};

extern std::vector<boost::shared_ptr<Object> > Objects;

boost::shared_ptr<Object> generateObject(
            Posture *posture,
            Renderer *renderer = nullptr, RigidBody *rigidbody = nullptr, Animation *animation = nullptr);

#endif // OBJECT_H
