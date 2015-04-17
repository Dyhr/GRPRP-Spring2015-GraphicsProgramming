#include "stdafx.h"
#include "AmbientLight.h"

namespace RayTracer
{
	AmbientLight::AmbientLight() : intensity(0.0f)
	{

	}

	AmbientLight::AmbientLight(float _intensity) : intensity(_intensity)
	{

	}

	LightType AmbientLight::getLightType()
	{
		return AMBIENT;
	}
	Vector3d AmbientLight::GetLightOnPoint(Point3d position)
	{
		return Vector3d();
	}

	float AmbientLight::GetIntensityOnPoint(Point3d position)
	{
		return intensity;
	}
}
