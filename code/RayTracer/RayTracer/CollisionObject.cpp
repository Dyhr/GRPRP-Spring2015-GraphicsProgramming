#include "stdafx.h"
#include "CollisionObject.h"

namespace RayTracer{
	CollisionObject::CollisionObject() : isReal(false)
	{
	}
	CollisionObject::CollisionObject(Object3d* objectHit, Point3d collisonHit) : object(objectHit), collisionCoord(collisonHit), isReal(true)
	{
		
	}
}

