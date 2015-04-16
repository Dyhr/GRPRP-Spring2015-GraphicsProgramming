#pragma once
#include "LightBase.h"
namespace RayTracer
{
	class PositionalLight : public LightBase
	{
		Vector3d centerPosition;	// Denotes center of lightsource
		float intensity;

	public:
		PositionalLight();
		PositionalLight(float intensity, Vector3d position);

		Vector3d GetLightOnPoint(Vector3d position);
		float GetIntensityOnPoint(Vector3d position);
	};
}

