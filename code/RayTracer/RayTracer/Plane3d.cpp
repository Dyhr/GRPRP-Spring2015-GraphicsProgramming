#include "stdafx.h"
#include "Plane3d.h"

namespace RayTracer
{
	Plane3d::Plane3d(Point3d point, Vector3d normal, vector<ShaderBase*> shaders) 
		: position(point), normal(normal), Object3d(shaders)
	{
	}

	Vector3d Plane3d::CalculateNormal(Vector3d) {
		return normal;
	}
	Vector3d Plane3d::CalculateCollisionPosition(Line3d) {
		return Vector3d();
	}
}
