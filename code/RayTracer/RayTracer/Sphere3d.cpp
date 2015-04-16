#include "stdafx.h"
#include "Sphere3d.h"
#include <cmath>

#include <iostream>

namespace RayTracer{
	Sphere3d::Sphere3d() : centerPosition(Vector3d(0.0, 0.0, 0.0)), radius(float(0.0))
	{

	}

	Sphere3d::Sphere3d(Vector3d center, float _radius, ShaderBase* shader) : centerPosition(center), radius(_radius), shader(shader)
	{

	}

	ShaderBase* Sphere3d::getShader(){
		return shader;
	}

	// Returns (for now) vector that is normal to the sphere through point.
	// The returned vector is normalized
	Vector3d Sphere3d::CalculateNormal(Vector3d pointAtSphere)
	{
		float x = pointAtSphere.x - centerPosition.x;
		float y = pointAtSphere.y - centerPosition.y;
		float z = pointAtSphere.z - centerPosition.z;

		Vector3d initial = Vector3d(x, y, z); // TODO: use vector3ds constructor instead.
		Vector3d normalized = Vector3d::normalize(initial);

		return normalized;
	}

	Vector3d Sphere3d::CalculateCollisionPosition(Line3d line)
	{
		// Notice, that there may be two intersections here, but only the "first" is returned

		//	As this method may be given a Line3d instance that does not originate from (0,0,0) a translation is made
		//	to ease the calculation
		

		// Move from spot to origin
		Vector3d translatedCenter = Vector3d::subtract(centerPosition,line.position);

		// Do calculations
		float a = Vector3d::dotProduct(line.direction,line.direction);
		float b = 2.0f*(Vector3d::dotProduct(line.direction, Vector3d::negate(translatedCenter)));
		float c = Vector3d::dotProduct(translatedCenter, translatedCenter) - (radius*radius);

		float d = pow(b, 2.0f) - 4.0f*a*c;

		// For now, arbitrarily chosen threshold for "0"
		float threshold = 0.01f;
		if (d > threshold)
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
		else if (d > 0.0 && d < threshold)
		{
			// One solution: (-b)/(2*a)
			float tValue = (-1.0f*b) / (2.0f*a);

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
