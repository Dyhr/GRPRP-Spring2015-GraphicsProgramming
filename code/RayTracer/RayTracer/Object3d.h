#pragma once
#include "Line3d.h"
#include "Vector3d.h"
#include "Color.h"
#include "LightBase.h"
#include <vector>
using namespace std;

namespace RayTracer{
	ref class Object3d
	{
	public:
		virtual Vector3d CalculateNormal(Vector3d) = 0; // normal based on position.. what?
		virtual Vector3d CalculateCollisionPosition(Line3d) = 0;
		virtual ColorIntern shadeThis(Vector3d eyeVector, Vector3d normalToSurface, Vector3d pointOnObject, vector<LightBase*> lights) = 0;
	};
}
