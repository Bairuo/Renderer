#ifndef BVH_H
#define BVH_H

#include <glm/vec3.hpp>
#include <list>
#include <iostream>

#define MAXDETECTNUM 100

#include "Object.h"
#include "Transform.h"

struct BoundingSphere
{
    glm::vec3 centre;
    double radius;

public:
    BoundingSphere(const glm::vec3 &centre, double radius);
    BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);

    bool overlaps(const BoundingSphere &other) const;

    double getGrowth(const BoundingSphere &other) const;

    double getSize() const
    {
        return (double)1.333333 * 3.141592 * radius * radius * radius;
    }
};

struct PotentialContact
{
    Object* body[2];
};

template<class BoundingVolumeClass>
class BVHNode
{
public:
    BVHNode* children[2];

    /**
     * Holds the rigid body at this node of the hierarchy.
     * Only leaf nodes can have a rigid body defined (see isLeaf).
     * Note that it is possible to rewrite the algorithms in this
     * class to handle objects at all levels of the hierarchy,
     * but the code provided ignores this vector unless firstChild
     * is NULL.
     */

    BVHNode* parent;

    BoundingVolumeClass volume;

    Object* body;

    BVHNode(BVHNode *parent, const BoundingVolumeClass &volume,
            Object* body = NULL)
        : parent(parent), volume(volume), body(body)
    {
        children[0] = children[1] = NULL;
    }

    bool isLeaf() const
    {
        return (body != NULL);
    }

    unsigned getPotentialContacts(PotentialContact* contacts,
                                  unsigned limit = MAXDETECTNUM) const;


    void insert(Object* body, const BoundingVolumeClass &volume);

    bool RemoveDirty(std::list<Object *> &removeObjs, std::list<BoundingVolumeClass> &removeVolumes);

    void Update(BVHNode<BoundingVolumeClass>* &root);

    ~BVHNode();

protected:
    bool overlaps(const BVHNode<BoundingVolumeClass> *other) const;

    unsigned getPotentialContactsWith(
        const BVHNode<BoundingVolumeClass> *other,
        PotentialContact* contacts,
        unsigned limit) const;

    void recalculateBoundingVolume(bool recurse = true);
};

template<class BoundingVolumeClass>
bool BVHNode<BoundingVolumeClass>::overlaps(
    const BVHNode<BoundingVolumeClass> * other
) const
{
    return volume.overlaps(other->volume);
}

template<class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::insert(
    Object* newBody, const BoundingVolumeClass &newVolume
)
{
    if (isLeaf())
    {
        children[0] = new BVHNode<BoundingVolumeClass>(
            this, volume, body
        );

        children[1] = new BVHNode<BoundingVolumeClass>(
            this, newVolume, newBody
        );

        this->body = NULL;

        recalculateBoundingVolume();

        //std::cout << "insert " << this << std::endl;
        //std::cout << "volume size " << volume.radius << std::endl;
    }
    else
    {
        if (children[0]->volume.getGrowth(newVolume) <
                children[1]->volume.getGrowth(newVolume))
        {
            children[0]->insert(newBody, newVolume);
        }
        else
        {
            children[1]->insert(newBody, newVolume);
        }
    }
}

template<class BoundingVolumeClass>
bool BVHNode<BoundingVolumeClass>::RemoveDirty(std::list<Object *> &removeObjs, std::list<BoundingVolumeClass> &removeVolumes)
{
    if(isLeaf() && body->dirty)
    {
        body->dirty = false;

        removeObjs.push_back(body);
        removeVolumes.push_back(volume);

        delete this;

        return true;
    }

    if(children[0])
    {
        if(children[0]->RemoveDirty(removeObjs, removeVolumes))
        {
            return RemoveDirty(removeObjs, removeVolumes);
        }
    }

    if(children[1])
    {
        children[1]->RemoveDirty(removeObjs, removeVolumes);
    }

    return false;
}

template<class BoundingVolumeClass>
void debug(BVHNode<BoundingVolumeClass>* node)
{
    std::cout << "-----" << std::endl;
    std::cout << "now " << node << std::endl;
    std::cout << "son1 " << node->children[0] << std::endl;
    std::cout << "son2 " << node->children[1] << std::endl;

    if(node->children[0] != NULL)debug(node->children[0]);
    if(node->children[1] != NULL)debug(node->children[1]);
}

template<class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::Update(BVHNode<BoundingVolumeClass>* &root)
{
    std::list<Object *> removeObjs;
    std::list<BoundingVolumeClass> removeVolumes;

    if(RemoveDirty(removeObjs, removeVolumes))
    {
        auto volume = *(removeVolumes.begin());
        volume.centre = (*(removeObjs.begin()))->transform->transVec3;

        root = new BVHNode<BoundingVolumeClass>(NULL, volume, *(removeObjs.begin()));

        removeObjs.erase(removeObjs.begin());
        removeVolumes.erase(removeVolumes.begin());
    }

    auto it1 = removeObjs.begin();
    auto it2 = removeVolumes.begin();

    while(it1 != removeObjs.end() && it2 != removeVolumes.end())
    {
        (*it2).centre = (*it1)->transform->transVec3;

        root->insert(*it1, *it2);

        it1 = removeObjs.erase(it1);
        it2 = removeVolumes.erase(it2);
    }
}

template<class BoundingVolumeClass>
BVHNode<BoundingVolumeClass>::~BVHNode()
{
    // If we don't have a parent, then we ignore the sibling
    // processing
    if (parent)
    {
        BVHNode<BoundingVolumeClass> *sibling;

        if (parent->children[0] == this) sibling = parent->children[1];
        else sibling = parent->children[0];

        parent->volume = sibling->volume;
        parent->body = sibling->body;
        parent->children[0] = sibling->children[0];
        parent->children[1] = sibling->children[1];

        // !! Bug on book
        if(sibling->children[0])sibling->children[0]->parent = parent;
        if(sibling->children[1])sibling->children[1]->parent = parent;

        sibling->parent = NULL;
        sibling->body = NULL;
        sibling->children[0] = NULL;
        sibling->children[1] = NULL;
        delete sibling;

        // Recalculate the parent's bounding volume
        parent->recalculateBoundingVolume();
    }

    // Delete our children (again we remove their
    // parent data so we don't try to process their siblings
    // as they are deleted).
    if (children[0])
    {
        children[0]->parent = NULL;
        delete children[0];
    }
    if (children[1])
    {
        children[1]->parent = NULL;
        delete children[0];
    }
}

template<class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::recalculateBoundingVolume(
    bool recurse
)
{
    if(isLeaf()) return;

    volume = BoundingVolumeClass(
                 children[0]->volume,
                 children[1]->volume
             );

    if(parent) parent->recalculateBoundingVolume(true);
}

template<class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::getPotentialContacts(
    PotentialContact* contacts, unsigned limit
) const
{
    if (isLeaf() || limit == 0) return 0;

    unsigned count1 = children[0]->getPotentialContacts(contacts, limit);
    unsigned count2 = children[1]->getPotentialContacts(contacts + count1, limit - count1);
    unsigned count3 = children[0]->getPotentialContactsWith(children[1], contacts + count1 + count2, limit - count1 - count2);

    return count1 + count2 + count3;
}

template<class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::getPotentialContactsWith(
    const BVHNode<BoundingVolumeClass> *other,
    PotentialContact* contacts,
    unsigned limit
) const
{
    if (!overlaps(other) || limit == 0) return 0;

    if (isLeaf() && other->isLeaf())
    {
        contacts->body[0] = body;
        contacts->body[1] = other->body;
        return 1;
    }

    if (other->isLeaf() ||
            (!isLeaf() && volume.getSize() >= other->volume.getSize()))
    {
        unsigned count = children[0]->getPotentialContactsWith(
                             other, contacts, limit
                         );

        if (limit > count)
        {
            return count + children[1]->getPotentialContactsWith(
                       other, contacts + count, limit - count
                   );
        }
        else
        {
            return count;
        }
    }
    else
    {
        // Recurse into the other node
        unsigned count = getPotentialContactsWith(
                             other->children[0], contacts, limit
                         );

        if (limit > count)
        {
            return count + getPotentialContactsWith(
                       other->children[1], contacts + count, limit - count
                   );
        }
        else
        {
            return count;
        }
    }
}



#endif // BVH_H_
