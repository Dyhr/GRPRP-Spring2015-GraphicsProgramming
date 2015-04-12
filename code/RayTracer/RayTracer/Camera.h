#pragma once
#include "Line3d.h"
namespace RayTracer{
	ref class Camera
	{
	public:
		Camera();
		Line3d GenerateRays(); // generate an twoDimensional array corrosponing to the render Array
	};
}

