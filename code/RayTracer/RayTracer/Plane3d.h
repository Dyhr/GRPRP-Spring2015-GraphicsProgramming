#pragma once
#include "Object3d.h"
#include "Point3d.h"
#include "Vector3d.h"
#include "ShaderBase.h"

namespace RayTracer
{
	class Plane3d : public Object3d
	{
	private:
		const Vector3d normal;
		const Point3d position;
	public:
		Plane3d(Point3d point, Vector3d normal, vector<ShaderBase*> shaders);

		Vector3d CalculateNormal(Point3d); // normal based on position always the same for a plane.
		Point3d CalculateCollisionPosition(Line3d);
	};

}

