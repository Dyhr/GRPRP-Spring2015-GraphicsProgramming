#pragma once
#include "Line3d.h"
#include "Vector3d.h"
namespace RayTracer{
	ref class Object3dBase
	{
	public:
		Object3dBase();
		virtual Vector3d CalculateNormal(Vector3d); // normal based on position
		virtual Vector3d CalculateCollisionPosition(Line3d);
	};
}

