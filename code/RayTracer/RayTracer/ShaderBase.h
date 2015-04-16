#pragma once
#include "Scene.h"
#include "LightBase.h"
#include <vector>

using namespace std;

namespace RayTracer
{
	class ShaderBase
	{

	public:
		virtual ColorIntern shade(Vector3d eyeVector, Vector3d normalToSurface, Vector3d pointOnObject, vector<LightBase*> lights) = 0;
	};
}