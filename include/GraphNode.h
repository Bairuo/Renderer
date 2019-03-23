// GraphNode.h
// Author: Bairuo

#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>

class Shader;
class Object;

class GraphNode
{
public:
    int objID;

    GraphNode* parent = nullptr;

    std::list<GraphNode *>sons;

    GraphNode();

    GraphNode(Object *obj);

    void addSubObject(Object *obj);

    void update(const glm::mat4 &parentMatrix, bool dirty);

    // clear

    // delete

private:
    void addSon(GraphNode *son);
};

extern GraphNode SceneGraph;

#endif // GRAPHNODE_H_
