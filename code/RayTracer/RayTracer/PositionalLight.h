#pragma once
#include "LightBase.h"
namespace RayTracer
{
	class PositionalLight : public LightBase
	{
		Point3d centerPosition;	// Denotes center of lightsource
		float intensity;

	public:
		PositionalLight();
		PositionalLight(float intensity, Point3d position);

		Vector3d GetLightOnPoint(Point3d position);
		float GetIntensityOnPoint(Point3d position);
		LightType getLightType();
	};
}

