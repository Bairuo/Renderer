#include "GraphNode.h"
#include "Shader.h"
#include "Object.h"
#include <queue>

GraphNode::GraphNode()
    :dirty(true),
     objID(-1)
{

}

GraphNode::GraphNode(const Object &obj)
    :dirty(true),
     objID(obj.id)
{

}

void GraphNode::addSon(GraphNode *son)
{
    son->parent = this;

    sons.push_back(son);
}

void GraphNode::addSubObject(const Object &obj)
{
    addSon(new GraphNode(obj));
}

void GraphNode::update()
{
    std::queue<GraphNode *> updateQueue;

    updateQueue.push(this);

    while(updateQueue.size() > 0)
    {

    }
}

void GraphNode::render()
{

}

void GraphNode::render(Shader &shader)
{

}
