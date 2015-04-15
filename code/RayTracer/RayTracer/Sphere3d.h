#ifndef SPHERE3D_H
#define SPHERE3D_H

#pragma once
#include "Object3d.h"
#include "Vector3d.h"
namespace RayTracer{

	class Sphere3d : public Object3d
	{
		const Vector3d centerPosition;
		const float radius;
	public:
		// Default constructor
		Sphere3d();
		// Fancy constructor
		Sphere3d(Vector3d center, float _radius);

		Vector3d CalculateNormal(Vector3d); // normal based on position
		Vector3d CalculateCollisionPosition(Line3d);
	};
}

#endif
