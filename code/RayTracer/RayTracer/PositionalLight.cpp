#include "stdafx.h"
#include "PositionalLight.h"

namespace RayTracer
{
	PositionalLight::PositionalLight() : intensity(0.0f), centerPosition(Point3d())
	{
	}

	PositionalLight::PositionalLight(float intensity, Point3d position) : intensity(intensity), centerPosition(position)
	{

	}

	LightType PositionalLight::getLightType()
	{
		return POSITIONAL;
	}

	Vector3d PositionalLight::GetLightOnPoint(Point3d position)
	{
		// Return vector going from lightsource to position
		return Vector3d(position, centerPosition);
	}

	float PositionalLight::GetIntensityOnPoint(Point3d position)
	{
		return intensity;
	}
}
