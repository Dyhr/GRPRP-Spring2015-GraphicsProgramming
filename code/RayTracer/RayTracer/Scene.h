#pragma once
#include "Line3d.h"
#include "Object3d.h"
#include "CollisionObject.h"
#include "Color.h"

namespace RayTracer {

	public ref class Scene {
	public:
		Scene(float, float, float); // Constructor
		void init(const int width, const int height);
		array<Color^>^ render();
		ColorIntern backgroundColor();
		ColorIntern ambientColorOnObjects();
	private:
		int width, height;
		float viewPortWidth, viewPortHeight, zLocation;
		array<Color^>^ arr;

		Color^ getColor(int x, int y);
		void setColor(int x, int y, Color^ color);
		Line3d getRayFromScreen(int x, int y);
		CollisionObject findClosestObject(Line3d ray);
		ColorIntern rayTrace(Line3d ray);
		void initializeSceneObjects();
	};
}
