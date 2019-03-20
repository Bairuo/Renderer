// GraphNode.h
// Author: Bairuo

#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <list>

class Shader;

class GraphNode
{
public:
    bool dirty;

    int objID;

    GraphNode* parent = nullptr;

    std::list<GraphNode *>sons;

    GraphNode();

    void addObject(int objID);

    void addSon(GraphNode *son);

    void update();

    void render();

    void render(Shader &shader);
};

#endif // GRAPHNODE_H_
