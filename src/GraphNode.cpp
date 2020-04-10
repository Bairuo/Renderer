#include "GraphNode.h"
#include "Shader.h"
#include "Object.h"
#include "Transform.h"
#include <queue>
#include "SceneConfig.h"
#include <iostream>

GraphNode::GraphNode()
    :objID(-1)
{

}

GraphNode::GraphNode(Object *obj)
    :objID(obj->id)
{
    obj->graphNode = this;
}

void GraphNode::addSon(GraphNode *son)
{
    son->parent = this;

    sons.push_back(son);
}

void GraphNode::setParentObject(Object *obj)
{
    for(auto iter = parent->sons.begin(); iter != parent->sons.end(); ++iter)
    {
        if(*iter == this)
        {
            parent->sons.erase(iter);
            break;
        }
    }

    parent = obj->graphNode;

    parent->sons.push_back(this);

    Objects[objID]->transform->setParentMatrix(obj->transform->getMatrix());
}

void GraphNode::addSubObject(Object *obj)
{
    if(objID != -1)
    {
        obj->transform->setParentMatrix(Objects[objID]->transform->getMatrix());
    }

    addSon(new GraphNode(obj));
}

void GraphNode::update(const glm::mat4 &parentMatrix, bool dirty)
{
    glm::mat4 thisMatrix = glm::mat4(1);

    if(objID != -1)
    {
        if(dirty)
        {
            Objects[objID]->transform->setParentMatrix(parentMatrix);
        }

        Objects[objID]->update();
        thisMatrix = Objects[objID]->transform->getMatrix();

        dirty |= Objects[objID]->dirty;
    }

    for(auto iter = sons.begin(); iter != sons.end(); ++iter)
    {
        (*iter)->update(thisMatrix, dirty);
    }
}
