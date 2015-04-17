#include "stdafx.h"
#include "DirectionalLight.h"

namespace RayTracer
{
	DirectionalLight::DirectionalLight() : intensity(0.0f), direction(Vector3d())
	{

	}

	DirectionalLight::DirectionalLight(float intensity, Vector3d direction) : intensity(intensity), direction(direction)
	{

	}

	LightType DirectionalLight::getLightType()
	{
		return DIRECTIONAL;
	}

	Vector3d DirectionalLight::GetLightOnPoint(Point3d position)
	{
		return direction;
	}

	float DirectionalLight::GetIntensityOnPoint(Point3d position)
	{
		return intensity;
	}
}

