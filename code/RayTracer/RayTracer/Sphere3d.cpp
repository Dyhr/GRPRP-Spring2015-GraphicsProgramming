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
		float translatedX = centerPosition.x - line.position.x;
		float translatedY = centerPosition.y - line.position.y;
		float translatedZ = centerPosition.z - line.position.z;
		Vector3d translatedCenter = Vector3d(translatedX, translatedY, translatedZ);
		Sphere3d translatedSphere = Sphere3d(translatedCenter,radius);

		// Do calculations
		float aComponent = pow(line.direction.x, 2) + pow(line.direction.y, 2) + pow(line.direction.z, 2);
		float bComponent = -2.0f*(line.direction.x*translatedCenter.x
			+ line.direction.y*translatedCenter.y
			+ line.direction.z*translatedCenter.z);
		float cComponent = pow(translatedCenter.x, 2) + pow(translatedCenter.y, 2) + pow(translatedCenter.z, 2);


		float discriminant = pow(bComponent, 2.0f) - 4.0f*aComponent*cComponent;

		float a = aComponent;
		float b = bComponent;
		float c = cComponent;


		// For now, arbitrarily chosen threshold for "0"
		float threshold = 0.01f;
		if (discriminant > threshold)
		{
			// Two solutions:
			float tValue1 = (-1.0f*b + sqrt(pow(b, 2) - 4.0f*a*c)) / (2.0f*a);
			float tValue2 = (-1.0f*b - sqrt(pow(b, 2) - 4.0f*a*c)) / (2.0f*a);

			// The lowest tValue corresponds to the first intersection
			float lowestTValue = tValue1 < tValue2 ? tValue1 : tValue2;

			// First solution: (-(b)+sqrt(D))/(2*a)
			float intersectX1 = lowestTValue * line.direction.x;
			float intersectY1 = lowestTValue * line.direction.y;
			float intersectZ1 = lowestTValue * line.direction.z;

			// Make up for initial translation
			intersectX1 = intersectX1 + line.position.x;
			intersectY1 = intersectY1 + line.position.y;
			intersectZ1 = intersectZ1 + line.position.z;

			Vector3d point = Vector3d(intersectX1, intersectY1, intersectZ1);

			return point;
		}
		else if (discriminant > 0.0 && discriminant < threshold)
		{
			// One solution: (-b)/(2*a)
			float tValue = (-1.0f*bComponent) / (2.0f*aComponent);

			// Corresponding point on line with given t
			float intersectX = tValue * line.direction.x;
			float intersectY = tValue * line.direction.y;
			float intersectZ = tValue * line.direction.z;

			// Make up for initial translation
			intersectX = intersectX + line.position.x;
			intersectY = intersectY + line.position.y;
			intersectZ = intersectZ + line.position.z;

			Vector3d point = Vector3d(intersectX, intersectY, intersectZ);
			return point;
		}
		else
		{
			// No solutions: Return empty array
			return Vector3d(); // TODO return null
		}
	}
}
