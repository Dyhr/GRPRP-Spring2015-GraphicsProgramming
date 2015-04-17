#pragma once
#include "Stdafx.h"
#include "Line3d.h"
#include "Vector3d.h"
#include "Color.h"
#include "LightBase.h"
#include "ShaderBase.h"
#include <vector>

using namespace std;

namespace RayTracer{
	class Object3d
	{
	public:
		const vector<ShaderBase*> shaders;

		Object3d(vector<ShaderBase*> shaders) : shaders(shaders) { }

		virtual Vector3d CalculateNormal(Vector3d) = 0; // normal based on position.. what?
		virtual Vector3d CalculateCollisionPosition(Line3d) = 0;

		ColorIntern shadeThis(Vector3d eyeVector, Vector3d normalToSurface, Vector3d pointOnObject, vector<LightBase*> lights) {
			ColorIntern colorToReturn = ColorIntern();

			for each (ShaderBase* shader in shaders) {
				colorToReturn = ColorIntern::blendAddition(colorToReturn, shader->shade(eyeVector, normalToSurface, pointOnObject, lights));
			}

			return colorToReturn;
		}
	};
}
