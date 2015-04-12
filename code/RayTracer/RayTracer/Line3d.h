#pragma once
#include "Vector3d.h"
namespace RayTracer{
	ref class Line3d
	{
	public:
		const Vector3d positionVector, directionVector;
		
		Line3d(); // empty constructor
		Line3d(Vector3d, Vector3d);

		Vector3d getPositionAlongLine(float);
		Line3d pushStartAlongLine(float);
	};
}


