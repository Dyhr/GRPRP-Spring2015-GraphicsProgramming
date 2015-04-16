#include "stdafx.h"
#include "PositionalLight.h"

namespace RayTracer
{
	PositionalLight::PositionalLight() : intensity(0.0f), centerPosition(Vector3d())
	{
	}

	PositionalLight::PositionalLight(float intensity, Vector3d position) : intensity(intensity), centerPosition(position)
	{

	}

	LightType PositionalLight::getLightType()
	{
		return POSITIONAL;
	}

	Vector3d PositionalLight::GetLightOnPoint(Vector3d position)
	{
		// Return vector going from lightsource to position
		return Vector3d(position, centerPosition);
	}

	float PositionalLight::GetIntensityOnPoint(Vector3d position)
	{
		return intensity;
	}
}
