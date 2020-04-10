#include "SceneConfig.h"
#include "Object.h"
#include "Transform.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "Animation.h"
#include "GraphNode.h"

static int ObjectID = 0;

std::map<int, boost::shared_ptr<Object> > Objects;
GraphNode SceneGraph;
BVHNode<BoundingSphere> *BVHTree = nullptr;

void BVHTreeBuild(BVHNode<BoundingSphere> *&BVHTree, boost::shared_ptr<Object> &object)
{
	if (object->renderer.get() != nullptr)
	{
		/**
		 * Default Contact test. Note even the object has not a rigidbody,
		 * it can also being tested(like Trigger test).
		 */
		double bRadius = object->renderer->getSphereBoundingRadius();

		if (bRadius > 0)
		{
			BoundingSphere bSphere(object->transform->getPosition(), bRadius);

			if (BVHTree == nullptr)
			{
				BVHTree = new BVHNode<BoundingSphere>(NULL, bSphere, object.get());
			}
			else
			{
				BVHTree->insert(object.get(), bSphere);
			}
		}
	}
}

boost::shared_ptr<Object> generateObject(Transform *transform, Renderer *renderer, RigidBody *rigidbody, Animation *animation)
{
	boost::shared_ptr<Object> newObj(new Object(ObjectID, transform, renderer, rigidbody, animation));

	if (transform != nullptr)
	{
		newObj->transform->obj = newObj.get();
	}

	if (renderer != nullptr)
	{
		newObj->renderer->obj = newObj.get();
	}

	if (rigidbody != nullptr)
	{
		newObj->rigidbody->obj = newObj.get();
	}

	Objects.insert(std::pair<int, boost::shared_ptr<Object> >(ObjectID, newObj));
	SceneGraph.addSubObject(newObj.get());

	ObjectID++;

	return newObj;
}
