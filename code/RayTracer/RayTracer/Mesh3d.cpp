#include "stdafx.h"
#include "Mesh3d.h"

namespace RayTracer {
	float min(float a, float b) {
		return a<b?a:b;
	}
	float max(float a, float b) {
		return a>b?a:b;
	}

	Mesh3d::Mesh3d(Point3d pos, vector<Triangle3d*> triangles, vector<ShaderBase*> shaders, Material material) 
		: pos(pos), triangles(triangles), boundingSphere(new Sphere3d(Point3d(), 0.0f)), Object3d(shaders, material)
	{
		Point3d* minp = new Point3d();
		Point3d* maxp = new Point3d();
		for each(Triangle3d* triangle in triangles) {
			minp = new Point3d(min(minp->x, triangle->a.x), min(minp->y, triangle->a.y), min(minp->z, triangle->a.z));
			maxp = new Point3d(max(maxp->x, triangle->a.x), max(maxp->y, triangle->a.y), max(maxp->z, triangle->a.z));
			minp = new Point3d(min(minp->x, triangle->b.x), min(minp->y, triangle->b.y), min(minp->z, triangle->b.z));
			maxp = new Point3d(max(maxp->x, triangle->b.x), max(maxp->y, triangle->b.y), max(maxp->z, triangle->b.z));
			minp = new Point3d(min(minp->x, triangle->c.x), min(minp->y, triangle->c.y), min(minp->z, triangle->c.z));
			maxp = new Point3d(max(maxp->x, triangle->c.x), max(maxp->y, triangle->c.y), max(maxp->z, triangle->c.z));
		}
		Point3d center = Point3d((minp->x + maxp->x) / 2, (minp->y + maxp->y) / 2, (minp->z + maxp->z) / 2);

		float radius = 0.0f;
		for each(Triangle3d* triangle in triangles) {
			radius = max(radius, Vector3d(triangle->a, center).length);
			radius = max(radius, Vector3d(triangle->b, center).length);
			radius = max(radius, Vector3d(triangle->c, center).length);

			triangle = new Triangle3d(triangle->a, triangle->b, triangle->c, shaders, material);
		}

		boundingSphere = new Sphere3d(center, radius);
	}

	Vector3d Mesh3d::CalculateNormal(Point3d point) {
		return Vector3d(0, 1, 0);
	}
	Point3d Mesh3d::CalculateCollisionPosition(Line3d line) {
		Point3d sphereHit = boundingSphere->CalculateCollisionPosition(line);
		if(sphereHit.x != 0 || sphereHit.y != 0 || sphereHit.z != 0) {
			Point3d* hit = new Point3d();
			float distance = 10000000.0f;
			for each(Triangle3d* triangle in triangles) {
				Point3d triHit = triangle->CalculateCollisionPosition(line);
				if(triHit.x != 0 || triHit.y != 0 || triHit.z != 0) {
					float d = Vector3d(Point3d(), triHit).length;
					if(d < distance) {
						hit = &triHit;
						distance = d;
					}
				}
			}
			return *hit;
		}
		return sphereHit;
	}
}
