#pragma once
#include "Object3d.h"
#include "Point3d.h"
#include "Vector3d.h"
#include "ShaderBase.h"

namespace RayTracer
{
	class Plane3d
	{
		const Vector3d normal;
		const Point3d position;
		ShaderBase* shader;
	public:
		Plane3d();
		Plane3d(Point3d point, Vector3d normal, ShaderBase* shader);

		Vector3d CalculateNormal(Vector3d); // normal based on position always the same for a plane.
		Vector3d CalculateCollisionPosition(Line3d);
		ColorIntern shadeThis(Vector3d eyeVector, Vector3d normalToSurface, Vector3d pointOnObject, vector<LightBase*> lights);
	};

}

