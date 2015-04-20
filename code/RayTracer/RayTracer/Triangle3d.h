#pragma once
#include "Object3d.h"
#include "Point3d.h"
#include "Vector3d.h"
#include "ShaderBase.h"

namespace RayTracer {
	class Triangle3d : public Object3d {
	private:
		const Point3d a, b, c;
		const Vector3d u, v, normal;
	public:
		Triangle3d(Point3d a, Point3d b, Point3d c, vector<ShaderBase*> shaders);
		Triangle3d(Point3d a, Point3d b, Point3d c, vector<ShaderBase*> shaders, Material material);

		Vector3d CalculateNormal(Point3d point);
		Point3d CalculateCollisionPosition(Line3d line);
	};
}

