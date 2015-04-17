#pragma once
#include "Object3d.h"
#include "Point3d.h"

namespace RayTracer{
	class CollisionObject
	{
	public:
		CollisionObject();
		CollisionObject(Object3d*, Point3d);
		Object3d* object;
		Point3d collisionCoord;
		bool isReal;
	};
}


