// GraphNode.h
// Author: Bairuo

#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <list>

class Shader;
class Object;

class GraphNode
{
public:
    bool dirty;

    int objID;

    GraphNode* parent = nullptr;

    std::list<GraphNode *>sons;

    GraphNode();

    GraphNode(const Object &obj);

    void addSon(GraphNode *son);

    void addSubObject(const Object &obj);

    void update();

    void render();

    void render(Shader &shader);
};

#endif // GRAPHNODE_H_
