#pragma once
#include "Object3dBase.h"
#include "Vector3d.h"
namespace RayTracer{
	ref class Sphere3d :
		public Object3dBase
	{
		const Vector3d centerPosition;
		const float radius;
	public:
		Sphere3d();
		Sphere3d(Vector3d, float);
	};
}


