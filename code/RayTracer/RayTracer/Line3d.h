#pragma once
#include "Vector3d.h"
namespace RayTracer{
	// Line3d should be thought of as a parametrization: (x(t),y(t),z(t)) = (x0,y0,z0) + t*(x1,y2,z2)
	class Line3d
	{
	public:
		const Vector3d positionVector, directionVector;
		
		Line3d(); // empty constructor
		Line3d(Vector3d, Vector3d);

		Vector3d getPositionAlongLine(float);
		Line3d pushStartAlongLine(float);
	};
}


