// SceneConfig.h
// Author: Bairuo

#ifndef SCENECONFIG_H_
#define SCENECONFIG_H_

#include <map>
#include <boost/shared_ptr.hpp>
#include"BVH.h"

class Object;
class Transform;
class Renderer;
class RigidBody;
class Animation;
class GraphNode;

extern std::map<int, boost::shared_ptr<Object> > Objects;
extern GraphNode SceneGraph;
extern BVHNode<BoundingSphere> *BVHTree;

boost::shared_ptr<Object> generateObject(
	Transform *transform,
	Renderer *renderer = nullptr, RigidBody *rigidbody = nullptr, Animation *animation = nullptr);

void BVHTreeBuild(BVHNode<BoundingSphere> *&BVHTree, boost::shared_ptr<Object> &object);

#endif // SCENECONFIG_H_
