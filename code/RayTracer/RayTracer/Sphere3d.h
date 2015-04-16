#pragma once
#include "Object3d.h"
#include "Vector3d.h"
#include "ShaderBase.h"

namespace RayTracer{
	class Sphere3d : public Object3d
	{
		const Vector3d centerPosition;
		const float radius;
		ShaderBase* shader;
		
	public:
		// Default constructor
		Sphere3d();
		// Fancy constructor
		Sphere3d(Vector3d center, float _radius, ShaderBase* shader);

		
		Vector3d CalculateNormal(Vector3d); // normal based on position
		Vector3d CalculateCollisionPosition(Line3d);
		ColorIntern shadeThis(Vector3d eyeVector, Vector3d normalToSurface, Vector3d pointOnObject, vector<LightBase*> lights);
	};
}
