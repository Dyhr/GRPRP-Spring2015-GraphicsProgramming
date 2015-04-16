#include "stdafx.h"
#include "Vector3d.h"
#include <cmath>

#define PI float(3.1415) 

namespace RayTracer
{

	// The empty constructor: Set default values for components
	Vector3d::Vector3d() : x(float(0.0)), y(float(0.0)), z(float(0.0)), length(float(0.0)) { }
	Vector3d::Vector3d(float x1, float y1, float z1) : x(x1), y(y1), z(z1), length(sqrt(pow(x1, 2) + pow(y1, 2) + pow(z1, 2))) { }
	Vector3d::Vector3d(Vector3d endPoint, Vector3d startPoint) : 
		x(float(endPoint.x - startPoint.x)), y(float(endPoint.y - startPoint.y)), 
		z(float(endPoint.z-startPoint.z)),
		length(float(sqrt(pow((endPoint.x - startPoint.x), 2) + pow((endPoint.y - startPoint.y), 2) + pow((endPoint.z - startPoint.z), 2)))) { }


	// Math

	Vector3d Vector3d::add(Vector3d a, Vector3d b) {
		return Vector3d(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	Vector3d Vector3d::subtract(Vector3d a, Vector3d b) {
		return Vector3d(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	Vector3d Vector3d::multiply(Vector3d a, float b) {
		return Vector3d(a.x * b, a.y * b, a.z * b);
	}

	Vector3d Vector3d::divide(Vector3d a, float b) {
		return Vector3d(a.x / b, a.y / b, a.z / b);
	}

	// Vector math

	Vector3d Vector3d::normalize(Vector3d v) {
		// We cannot divide by zero
		// Return itself
		if(v.length == 0.0f) return v;
		return divide(v, v.length);
	}

	Vector3d Vector3d::negate(Vector3d v) {
		return multiply(v, -1);
	}

	float Vector3d::dotProduct(Vector3d v1, Vector3d v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float Vector3d::distance(Vector3d v1, Vector3d v2) {
		return subtract(v1, v2).length;
	}

	float cosV(Vector3d v1, Vector3d v2) {
		// The two are normalized
		Vector3d normV1 = Vector3d::normalize(v1);
		Vector3d normV2 = Vector3d::normalize(v2);

		float dotP = Vector3d::dotProduct(normV1, normV2);
		return cos(dotP);
	}

	// Other math

	Vector3d Vector3d::reflectionVector(Vector3d normal, Vector3d incoming)
	{
		// Define exiting vector originating from the origin of normal
		// http://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
		Vector3d step1 = multiply(incoming, 2.0f);			// 2*a
		float step2 = dotProduct(step1, normal);					// 2*a*n
		float step3 = step2 / pow(normal.length, 2);				// (2*a*n)/(|n|^2)
		Vector3d step4 = multiply(normal, step3);			// ((2*a*n)/(|n|^2)) x n
		Vector3d final = subtract(incoming, step4);				// a - ( ((2*a*n)/(|n|^2)) x n )

		return final;
	}

	Vector3d Vector3d::refractionVector(Vector3d, Vector3d, float, float) // normal and an incoming - refractionindex of from and to material
	{
		// Dummy implementation: Haven't had time to look into this yet!
		return Vector3d();
	}

	// Notice, that isSameDirection returns false if v1 and v2 are parallel but pointing in opposite directions
	bool Vector3d::isSameDirection(Vector3d v1, Vector3d v2) {
		float threshold = 0.0001f;
		return abs(dotProduct(normalize(v1), normalize(v2))) > 1.0f - threshold;
	}
}
