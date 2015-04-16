#pragma once

#include "Vector3d.h"

namespace RayTracer
{
	enum LightType
	{
		AMBIENT,
		DIRECTIONAL,
		POSITIONAL
	};
	class LightBase
	{
	public:
		virtual LightType getLightType()=0;
		// Position denotes the point, where the light should shine on
		virtual Vector3d GetLightOnPoint(Vector3d position) = 0;
		virtual float GetIntensityOnPoint(Vector3d position) = 0;
	};
}


