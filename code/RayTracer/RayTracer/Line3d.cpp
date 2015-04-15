#include "stdafx.h"
#include "Line3d.h"

namespace RayTracer{

	// Default constructor
	Line3d::Line3d() : positionVector(Vector3d(0.0, 0.0, 0.0)), directionVector(Vector3d(0.0,0.0,0.0))
	{

	}

	Line3d::Line3d(Vector3d start, Vector3d direction) : positionVector(start), directionVector(direction)
	{

	}

	Vector3d Line3d::getPositionAlongLine(float t)
	{
		float x = positionVector.x + t * directionVector.x;
		float y = positionVector.y + t * directionVector.y;
		float z = positionVector.z + t * directionVector.z;

		Vector3d pointOnLine = Vector3d(x, y, z);
		return pointOnLine;
	}

	// Returns a new line with a modified starting point; direction will be the same
	Line3d Line3d::pushStartAlongLine(float pushAmount)
	{
		Vector3d newStartPoint = getPositionAlongLine(pushAmount);
		Line3d newLine = Line3d(newStartPoint, directionVector);
		return newLine;
	}
}
