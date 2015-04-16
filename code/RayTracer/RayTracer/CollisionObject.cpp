#include "stdafx.h"
#include "CollisionObject.h"

namespace RayTracer{
	CollisionObject::CollisionObject() : isReal(false)
	{
	}
	CollisionObject::CollisionObject(Object3d* objectHit, Vector3d collisonHit) : object(objectHit), collisionCoord(collisonHit), isReal(true)
	{
		
	}
}

