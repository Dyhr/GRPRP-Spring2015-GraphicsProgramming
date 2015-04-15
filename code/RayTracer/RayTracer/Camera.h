#pragma once
#include "Line3d.h"
namespace RayTracer{
	ref class Camera
	{
	public:
		// Camera is assumed to be located in (0,0,zLocation)
		Camera();
		Line3d* GenerateRays(); // generate an twoDimensional array corresponding to the render Array
	};
}

