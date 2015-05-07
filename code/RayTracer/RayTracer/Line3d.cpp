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
		vector<Line3d> lines = vector<Line3d>();
		lines.push_back(*this);

		
		for (size_t i = 0; i < amountOfLines; i++)
		{
			float randomX = (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * (float)twistAmount;
			float randomY = (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * (float)twistAmount;
			float randomZ = (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * (float)twistAmount;

			float dx = direction.x + randomX;
			float dy = direction.y + randomY;
			float dz = direction.z + randomZ;

			//randomX = ((float)rand() / (float)RAND_MAX) * twistAmount - halfTwist;
			//randomY = ((float)rand() / (float)RAND_MAX) * twistAmount - halfTwist;
			//randomZ = ((float)rand() / (float)RAND_MAX) * twistAmount - halfTwist;

			float px = position.x;// + randomX;
			float py = position.y;// + randomY;
			float pz = position.z;// + randomZ;

			//float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twistAmount));
			//float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twistAmount));
			//float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twistAmount));
			//
			//x = direction.x + (x - halfTwist);
			//y = direction.y + (y - halfTwist);
			//z = direction.z + (z - halfTwist);

			lines.push_back(Line3d(Point3d(px,py,pz),Vector3d(dx,dy,dz)));
		}
		return lines;
	}
}
