#include "stdafx.h"
#include "Sphere3d.h"
#include <cmath>

namespace RayTracer{
	Sphere3d::Sphere3d() : centerPosition(Vector3d(0.0, 0.0, 0.0)), radius(float(0.0))
	{

	}

	Sphere3d::Sphere3d(Vector3d center, float _radius) : centerPosition(center), radius(_radius)
	{

	}

	// Returns (for now) vector that is normal to the sphere through point.
	// The returned vector is normalized
	Vector3d Sphere3d::CalculateNormal(Vector3d pointAtSphere)
	{
		float x = pointAtSphere.x - centerPosition.x;
		float y = pointAtSphere.y - centerPosition.y;
		float z = pointAtSphere.z - centerPosition.z;

		Vector3d initial = Vector3d(x, y, z);
		Vector3d normalized = Vector3d::normalize(initial);

		return normalized;
	}

	Vector3d Sphere3d::CalculateCollisionPosition(Line3d line)
	{
		// Notice, that there may be two intersections here, but only the "first" is returned

		//	As this method may be given a Line3d instance that does not originate from (0,0,0) a translation is made
		//	to ease the calculation
		

		// Move from spot to origin
		float translatedX = centerPosition.x - line.positionVector.x;
		float translatedY = centerPosition.y - line.positionVector.y;
		float translatedZ = centerPosition.z - line.positionVector.z;
		Vector3d translatedCenter = Vector3d(translatedX, translatedY, translatedZ);
		Sphere3d translatedSphere = Sphere3d(translatedCenter,radius);

		// Do calculations
		float aComponent = pow(line.directionVector.x, 2) + pow(line.directionVector.y, 2) + pow(line.directionVector.z, 2);
		float bComponent = -2.0*(line.directionVector.x*translatedCenter.x
			+ line.directionVector.y*translatedCenter.y
			+ line.directionVector.z*translatedCenter.z);
		float cComponent = pow(translatedCenter.x, 2) + pow(translatedCenter.y, 2) + pow(translatedCenter.z, 2);


		float discriminant = pow(bComponent, 2.0) - 4.0*aComponent*cComponent;

		float a = aComponent;
		float b = bComponent;
		float c = cComponent;


		// For now, arbitrarily chosen threshold for "0"
		float threshold = 0.01;
		if (discriminant > threshold)
		{
			// Two solutions:
			float tValue1 = (-1.0*b + sqrt(pow(b, 2) - 4.0*a*c)) / (2.0*a);
			float tValue2 = (-1.0*b - sqrt(pow(b, 2) - 4.0*a*c)) / (2.0*a);

			// The lowest tValue corresponds to the first intersection
			float lowestTValue = tValue1 < tValue2 ? tValue1 : tValue2;

			// First solution: (-(b)+sqrt(D))/(2*a)
			float intersectX1 = lowestTValue * line.directionVector.x;
			float intersectY1 = lowestTValue * line.directionVector.y;
			float intersectZ1 = lowestTValue * line.directionVector.z;

			// Make up for initial translation
			intersectX1 = intersectX1 + line.positionVector.x;
			intersectY1 = intersectY1 + line.positionVector.y;
			intersectZ1 = intersectZ1 + line.positionVector.z;

			Vector3d point = Vector3d(intersectX1, intersectY1, intersectZ1);

			return point;
		}
		else if (discriminant > 0.0 && discriminant < threshold)
		{
			// One solution: (-b)/(2*a)
			float tValue = (-1.0*bComponent) / (2.0*aComponent);

			// Corresponding point on line with given t
			float intersectX = tValue * line.directionVector.x;
			float intersectY = tValue * line.directionVector.y;
			float intersectZ = tValue * line.directionVector.z;

			// Make up for initial translation
			intersectX = intersectX + line.positionVector.x;
			intersectY = intersectY + line.positionVector.y;
			intersectZ = intersectZ + line.positionVector.z;

			Vector3d point = Vector3d(intersectX, intersectY, intersectZ);
			return point;
		}
		else
		{
			// No solutions: Return empty array
			return Vector3d();
		}
	}
}
