#pragma once
#include "Object3d.h"
#include "Vector3d.h"

namespace RayTracer{
	class CollisionObject
	{
	public:
		CollisionObject();
		CollisionObject(Object3d*, Vector3d);
		Object3d* object;
		Vector3d collisionCoord;
		bool isReal;
	};
}


