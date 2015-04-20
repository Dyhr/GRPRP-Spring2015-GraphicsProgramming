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
		Scene(int height, int width, float viewsize, float viewdistance);
		array<Color^>^ render();
		ColorIntern backgroundColor();
		ColorIntern ambientColorOnObjects();
		float sceneRefractionIndex = 1.0f;
	private:
		int width, height;
		float viewPortWidth, viewPortHeight, viewPortSize, zLocation;
		array<Color^>^ arr;

		Color^ getColor(int x, int y);
		void setColor(int x, int y, Color^ color);
		Line3d getRayFromScreen(int x, int y);
		CollisionObject findClosestObject(Line3d ray);
		ColorIntern rayTrace(Line3d ray, int count, float currentRefractionIndex);

		vector<LightBase*> getLightsThatHitPoint(Point3d point);
	};
}
