#pragma once
#include "LightBase.h"
#include "Vector3d.h"
namespace RayTracer
{
	class AmbientLight : public LightBase
	{
		float intensity;
	public:
		AmbientLight();
		AmbientLight(float intensity);
		Vector3d GetLightOnPoint(Point3d position);
		float GetIntensityOnPoint(Point3d position);
		LightType getLightType();
	};
}

