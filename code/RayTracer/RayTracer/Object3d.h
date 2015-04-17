#pragma once
#include "Line3d.h"
#include "Vector3d.h"
#include "Color.h"
#include "LightBase.h"
#include <vector>
using namespace std;

namespace RayTracer{
	class Object3d
	{
	public:
		virtual Vector3d CalculateNormal(Point3d) = 0; // normal based on position.. what?
		virtual Point3d CalculateCollisionPosition(Line3d) = 0;
		virtual ColorIntern shadeThis(Vector3d eyeVector, Vector3d normalToSurface, Point3d pointOnObject, vector<LightBase*> lights) = 0;
	};
}
