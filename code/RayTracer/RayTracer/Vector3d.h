#pragma once
namespace RayTracer{
	// The Vector3d class sh
	ref class Vector3d
	{
		const float x, y, z;
		const float length;
	
	public:
		Vector3d(); // the empty constructor.
		Vector3d(float, float, float);
		Vector3d(Vector3d, Vector3d);
		Vector3d normalizeVector();
		Vector3d negateVector();
		Vector3d vectorTimesScalar(float);

		static Vector3d addition(Vector3d, Vector3d);
		static Vector3d subtraction(Vector3d, Vector3d);
		static Vector3d multiplication(Vector3d, Vector3d);
		static Vector3d reflectionVector(Vector3d, Vector3d); // normal and an incoming
		static Vector3d refractionVector(Vector3d, Vector3d, float, float); // normal and an incoming - refractionindex of from and to material
		static float dotProduct(Vector3d, Vector3d);
		static float cosV(Vector3d, Vector3d);
		static float distanceBetweenPoints(Vector3d, Vector3d); 
		static bool isSameDirection(Vector3d, Vector3d);
	};
}


