#ifndef OBJECT3D_H
#define OBJECT3D_H

#pragma once
#include "Line3d.h"
#include "Vector3d.h"

namespace RayTracer{
	class Object3d
	{
	public:
		virtual Vector3d CalculateNormal(Vector3d); // normal based on position
		virtual Vector3d CalculateCollisionPosition(Line3d);
	};
}

#endif
