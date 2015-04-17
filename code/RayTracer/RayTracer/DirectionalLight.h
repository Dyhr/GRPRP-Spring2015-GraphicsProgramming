#pragma once
#include "LightBase.h"
namespace RayTracer
{
	class DirectionalLight : public LightBase
	{
		float intensity;
		Vector3d direction;
	public:
		DirectionalLight();
		DirectionalLight(float intensity, Vector3d direction);
		Vector3d GetLightOnPoint(Point3d position);
		float GetIntensityOnPoint(Point3d position);
		LightType getLightType();
	};
}


