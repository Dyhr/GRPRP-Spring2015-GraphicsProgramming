#pragma once
#include "Vector3d.h"
namespace RayTracer{
	ref class Object3dBase
	{
	public:
		Object3dBase();
		virtual Vector3d CalculateNormal();
		virtual Vector3d CalculateCollisionPosition();
	};
}

