#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <boost/shared_ptr.hpp>

class Transform;
class Renderer;
class RigidBody;
class Animation;
class Shader;
class GraphNode;

class Object
{
public:
    int id;
    bool dirty = false;     // If true, object need to be updated in Spatial Data Structure
    GraphNode *graphNode = nullptr;

    // Components
    // Can change to array
    // RigidBody *rigidbody[MAXSIZE]
    boost::shared_ptr<Transform> transform;
    boost::shared_ptr<Renderer> renderer;
    boost::shared_ptr<RigidBody> rigidbody;
    boost::shared_ptr<Animation> animation;

    Object(int id, Transform *transform, Renderer *renderer, RigidBody *rigidbody, Animation *animation);
    ~Object();

    void update();

    void render();

    void render(Shader &shader);

    void setParent(boost::shared_ptr<Object> obj);

    void addSubObject(boost::shared_ptr<Object> obj);

    // ...
};

#endif // OBJECT_H
