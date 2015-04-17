#include "stdafx.h"
#include "Plane3d.h"

namespace RayTracer
{
	Plane3d::Plane3d(Point3d point, Vector3d normal, vector<ShaderBase*> shaders) 
		: position(point), normal(normal), Object3d(shaders)
	{
	}

	Vector3d Plane3d::CalculateNormal(Point3d point) {
		return normal;
	}
	Point3d Plane3d::CalculateCollisionPosition(Line3d line) {
		if(Vector3d::dotProduct(line.direction, normal) == 0) return Point3d();

		float d = (Vector3d::dotProduct(Vector3d(line.position,position),normal)) / (Vector3d::dotProduct(line.direction, normal));

		return line.getPositionAlongLine(d);
	}
}
