#pragma once
namespace RayTracer{
	// The Vector3d class sh
	class Vector3d
	{
		public:
		// Denotes either a) a point in space (in which case length must be 0) or b) a vector with a direction
		const float x, y, z, length;
		Vector3d(); // the empty constructor.
		Vector3d(float, float, float);	// This represents a) a point or b) a vector
		Vector3d(Vector3d, Vector3d);	// This represents a parametrization: (x,y,z) = (x0,y0,z0) + t*(x1,y1,z1) <- Nope, Wrong! 
		Vector3d normalizeVector();
		Vector3d negateVector();
		Vector3d vectorTimesScalar(float);

		static Vector3d addition(Vector3d, Vector3d);

		// Important: subtraction() performs the subtraction v1 - v2; NOT v2 - v1
		static Vector3d subtraction(Vector3d v1, Vector3d v2);
		static Vector3d reflectionVector(Vector3d normal, Vector3d incoming); // normal and an incoming
		static Vector3d vectorTimesScalar(Vector3d vector, float scalar);
		static Vector3d refractionVector(Vector3d, Vector3d, float, float); // normal and an incoming - refractionindex of from and to material
		static Vector3d normalizeVector(Vector3d vectorToBeNormalized);
		static float dotProduct(Vector3d v1, Vector3d v2);
		// Determines cos(v), where v denotes the angle between v1 and v2
		static float cosV(Vector3d v1, Vector3d v2);
		static float distanceBetweenPoints(Vector3d, Vector3d); 
		static bool isSameDirection(Vector3d, Vector3d);
	};
}


