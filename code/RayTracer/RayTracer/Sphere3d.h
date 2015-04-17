#pragma once
#include "Object3d.h"
#include "Vector3d.h"
#include "ShaderBase.h"
#include "Point3d.h"

namespace RayTracer{
	class Sphere3d : public Object3d
	{
		const Point3d centerPosition;
		const float radius;
		
	public:
		Sphere3d(Point3d center, float _radius, vector<ShaderBase*> shaders);
		
		Vector3d CalculateNormal(Point3d); // normal based on position
		Point3d CalculateCollisionPosition(Line3d);
	};
}
