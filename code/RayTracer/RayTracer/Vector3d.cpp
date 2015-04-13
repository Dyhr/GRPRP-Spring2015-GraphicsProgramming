#include "stdafx.h"
#include "Vector3d.h"
#include <cmath>

#define PI float(3.14) 

namespace RayTracer
{

	// The empty constructor: Set default values for components
	Vector3d::Vector3d() : x(float(0.0)), y(float(0.0)), z(float(0.0)), length(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)))
	{

	}

	Vector3d::Vector3d(float x1, float y1, float z1) : x(x1), y(y1), z(z1), length(sqrt(pow(x1, 2) + pow(y1, 2) + pow(z1, 2)))
	{

	}

	Vector3d::Vector3d(Vector3d endPoint, Vector3d startPoint) : 
		x(float(endPoint.x - startPoint.x)), y(float(endPoint.y - startPoint.y)), 
		z(float(endPoint.z-startPoint.z)),
		length(float(sqrt(pow((endPoint.x - startPoint.x), 2) + pow((endPoint.y - startPoint.y), 2) + pow((endPoint.z - startPoint.z), 2))))
	{

	}

	Vector3d Vector3d::normalizeVector()
	{
		if (length == 0.0f)
		{
			// We cannot divide by zero
			// Return a zero-length vector
			Vector3d returnValue = Vector3d();
			return returnValue;
		}

		float normalisedX = x / length;
		float normalisedY = y / length;
		float normalisedZ = z / length;

		Vector3d normalisedVector = Vector3d(normalisedX, normalisedY, normalisedZ);
		return normalisedVector;
	}

	Vector3d Vector3d::negateVector()
	{
		float xNegated = x * (-1.0);
		float yNegated = y * (-1.0);
		float zNegated = z * (-1.0);

		Vector3d negatedVector = Vector3d(xNegated, yNegated, zNegated);
		return negatedVector;
	}

	Vector3d Vector3d::vectorTimesScalar(float scalar)
	{
		float scaledX = x * scalar;
		float scaledY = y * scalar;
		float scaledZ = z * scalar;

		Vector3d scaledVector = Vector3d(scaledX, scaledY, scaledZ);
		return scaledVector;
	}


	Vector3d Vector3d::addition(Vector3d v1, Vector3d v2)
	{
		float newX = v1.x + v2.x;
		float newY = v1.y + v2.y;
		float newZ = v1.z + v2.z;

		return Vector3d(newX, newY, newZ);
	}

	// Since, header-file declares this method as static, there's no need to repeat that info; http://stackoverflow.com/questions/15725922/static-function-a-storage-class-may-not-be-specified-here
	Vector3d Vector3d::subtraction(Vector3d v1, Vector3d v2)
	{
		float newX = v1.x - v2.x;
		float newY = v1.y - v2.y;
		float newZ = v1.z - v2.z;

		return Vector3d(newX, newY, newZ);
	}

	Vector3d Vector3d::reflectionVector(Vector3d normal, Vector3d incoming)
	{
		// Define exiting vector originating from the origin of normal
		// http://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
		Vector3d step1 = incoming.vectorTimesScalar(2.0);			// 2*a
		float step2 = dotProduct(step1, normal);					// 2*a*n
		float step3 = step2 / pow(normal.length, 2.0);				// (2*a*n)/(|n|^2)
		Vector3d step4 = vectorTimesScalar(normal, step3);			// ((2*a*n)/(|n|^2)) x n
		Vector3d final = subtraction(incoming, step4);				// a - ( ((2*a*n)/(|n|^2)) x n )

		return final;
	}

	Vector3d Vector3d::refractionVector(Vector3d, Vector3d, float, float) // normal and an incoming - refractionindex of from and to material
	{
		// Dummy implementation: Haven't had time to look into this yet!
		return Vector3d();
	}

	float dotProduct(Vector3d v1, Vector3d v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float cosV(Vector3d v1, Vector3d v2)
	{
		// The two are normalized
		Vector3d normV1 = v1.normalizeVector();
		Vector3d normV2 = v2.normalizeVector();

		float dotP = dotProduct(normV1, normV2);
		return cos(dotP);
	}

	float distanceBetweenPoints(Vector3d v1, Vector3d v2)
	{
		float x1 = v1.x;
		float y1 = v1.y;
		float z1 = v1.z;
		
		float x2 = v2.x;
		float y2 = v2.y;
		float z2 = v2.z;

		return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2) + pow((z2 - z1), 2));
	}

	// Notice, that isSameDirection returns false if v1 and v2 are parallel but pointing in opposite directions
	bool isSameDirection(Vector3d v1, Vector3d v2)
	{
		// TODO: Define global threshold value
		float threshold = 0.001;
		return abs(dotProduct(v1,v2) - 1.0 ) < threshold;
	}
}
