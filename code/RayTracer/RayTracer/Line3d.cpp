#include "stdafx.h"
#include "Line3d.h"

namespace RayTracer{

	// Default constructor
	Line3d::Line3d() : position(Point3d(0.0, 0.0, 0.0)), direction(Vector3d(0.0, 0.0, 0.0))
	{
		//srand(static_cast <unsigned> (time(0)));
	}

	Line3d::Line3d(Point3d position, Vector3d direction):position(position), direction(direction)
	{
		//srand(static_cast <unsigned> (time(0)));
	}

	Point3d Line3d::getPositionAlongLine(float t)
	{
		float x = position.x + t * direction.x;
		float y = position.y + t * direction.y;
		float z = position.z + t * direction.z;

		Point3d pointOnLine = Point3d(x, y, z);
		return pointOnLine;
	}

	// Returns a new line with a modified starting point; direction will be the same
	Line3d Line3d::pushStartAlongLine(float pushAmount)
	{
		Point3d newStartPoint = getPositionAlongLine(pushAmount);
		Line3d newLine = Line3d(newStartPoint, direction);
		return newLine;
	}

	vector<Line3d> Line3d::getTwistedLines(int amountOfLines, float twistAmount)
	{
		srand(time(NULL));

		vector<Line3d> lines = vector<Line3d>();
		lines.push_back(*this);

		float halfTwist = twistAmount / 2.0f;
		for (size_t i = 0; i < amountOfLines; i++)
		{ 
			float randomX = ((float)rand() / (float)RAND_MAX) * twistAmount;
			float randomY = ((float)rand() / (float)RAND_MAX) * twistAmount;
			float randomZ = ((float)rand() / (float)RAND_MAX) * twistAmount;

			float x = direction.x + (randomX - halfTwist);
			float y = direction.y + (randomY - halfTwist);
			float z = direction.z + (randomZ - halfTwist);

			//float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twistAmount));
			//float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twistAmount));
			//float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twistAmount));
			//
			//x = direction.x + (x - halfTwist);
			//y = direction.y + (y - halfTwist);
			//z = direction.z + (z - halfTwist);

			lines.push_back(Line3d(position,Vector3d(x,y,z)));
		}
		return lines;
	}
}
