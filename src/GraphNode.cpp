#include "GraphNode.h"
#include "Shader.h"

GraphNode::GraphNode()
    :dirty(false),
     objID(-1)
{

}

void GraphNode::addObject(int objID)
{

}

void GraphNode::addSon(GraphNode *son)
{
    son->parent = this;

    sons.push_back(son);
}

void GraphNode::update()
{

}

void GraphNode::render()
{

}

void GraphNode::render(Shader &shader)
{

}
