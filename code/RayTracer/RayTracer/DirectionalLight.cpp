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

	Vector3d DirectionalLight::GetLightOnPoint(Vector3d position)
	{
		return direction;
	}

	float DirectionalLight::GetIntensityOnPoint(Vector3d position)
	{
		return intensity;
	}
}

