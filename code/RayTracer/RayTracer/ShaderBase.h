#pragma once
#include "Scene.h"
#include "Color.h"
#include <vector>

using namespace std;

namespace RayTracer
{
	class ShaderBase
	{
	public:
		virtual ColorIntern shade(Vector3d eyeVector, Vector3d normalToSurface, Point3d pointOnObject, vector<LightBase*> lights) = 0;
	};
}