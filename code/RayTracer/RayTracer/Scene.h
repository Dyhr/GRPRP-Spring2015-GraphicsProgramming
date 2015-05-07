#pragma once
#include "Line3d.h"
#include "Object3d.h"
#include "CollisionObject.h"
#include "Color.h"
#include "LightBase.h"
#include <vector>
using namespace std;

namespace RayTracer {

	public ref class Scene {
	public:
		Scene(int height, int width, float viewdistance);
		array<Color^>^ render();
		ColorIntern backgroundColor();
		ColorIntern ambientColorOnObjects();
		float sceneRefractionIndex = 1.0f;
		int amtOfShadowRays = 90;   // set this to something higher to add soft shadows
	private:
		int width, height;
		float viewPortWidth, viewPortHeight, stepSizeX, stepSizeY, zLocation;
		array<Color^>^ arr;


		ColorIntern getReflectionColor(Vector3d normal, Vector3d incomingDirection, int count, Point3d hitPoint, float currentRefractionIndex);
		ColorIntern getRefractionColor(Vector3d normal, Vector3d incomingDirection, int count, Point3d hitPoint, float currentRefractionIndex, float nextRefraction, Material materialOfObject);

		Color^ getColor(int x, int y);
		void setColor(int x, int y, Color^ color);
		Line3d getRayFromScreen(int x, int y);
		CollisionObject findClosestObject(Line3d ray);
		ColorIntern rayTrace(Line3d ray, int count, float currentRefractionIndex);

		vector<LightBase*> getLightsThatHitPoint(Point3d point);
		vector<LightBase*> getLightsThatHitPointSoftShadows(Point3d point);


		void initLists();
		void setUpCornellBox();

		void TwoSpheresInCornellBox();
		void TrianglesInCornellBox();
		void MeshInCornellBox();
	};
}
