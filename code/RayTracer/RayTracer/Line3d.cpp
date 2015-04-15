#include "stdafx.h"
#include "Line3d.h"

namespace RayTracer{

	// Default constructor
	Line3d::Line3d() : position(Vector3d(0.0, 0.0, 0.0)), direction(Vector3d(0.0, 0.0, 0.0))
	{

	}

	Line3d::Line3d(Vector3d position, Vector3d direction):position(position), direction(direction)
	{

	}

	Vector3d Line3d::getPositionAlongLine(float t)
	{
		float x = position.x + t * direction.x;
		float y = position.y + t * direction.y;
		float z = position.z + t * direction.z;

		Vector3d pointOnLine = Vector3d(x, y, z);
		return pointOnLine;
	}

	// Returns a new line with a modified starting point; direction will be the same
	Line3d Line3d::pushStartAlongLine(float pushAmount)
	{
		Vector3d newStartPoint = getPositionAlongLine(pushAmount);
		Line3d newLine = Line3d(newStartPoint, direction);
		return newLine;
	}
}
