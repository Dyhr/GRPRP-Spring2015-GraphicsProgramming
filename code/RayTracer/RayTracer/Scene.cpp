// This is the main DLL file.

#include "stdafx.h"
#include <iostream>
#include <ctgmath>
#include <cmath>
#include <vector>

#include "Sphere3d.h"
#include "CollisionObject.h"
#include "Scene.h"

using namespace std;

namespace RayTracer {

	Color^ Scene::backgroundColor(){
		Color^ background = gcnew Color;
		background->alpha = 255;
		background->red = 255;
		background->green = 255;
		background->blue = 0;
		return background;
	}

	Scene::Scene(float viewPortWidth, float viewPortHeight, float zLocation)
	{
		this->viewPortWidth = viewPortWidth;
		this->viewPortHeight = viewPortHeight;
		this->zLocation = zLocation; // Denotes the (z)-depth at which the viewport is located. We assume that camera is located at (0,0,0)
	}
	void Scene::init(const int width, const int height) {
		this->width = width;
		this->height = height;
		this->arr = gcnew array<Color^>(width*height);

		for(int i = 0; i < width*height; i++) {
			this->arr[i] = gcnew Color;
		}
	}

	vector<Object3d*> sceneObjects;

	array<Color^>^ Scene::render()
	{
		// Things in the scene goes here for now
		sceneObjects = vector<Object3d*>(1);
		sceneObjects[0] = new Sphere3d(Vector3d(0,100,100),20);

		// This is where the magic happens: main-loop!
		for (int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++) {
				Line3d ray = getRayFromScreen(x, y);
				Color^ color = rayTrace(ray);
				setColor(x, y, color);
			}
		}

		// Garbage collection
		for(vector<Object3d*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
			delete *it;
		}

		return arr;
	}

	Color^ Scene::getColor(int x, int y) {
		return arr[x + y*width];
	}
	void Scene::setColor(int x, int y, Color^ color) {
		arr[x + y*width] = color;
	}
		
	// Assumes viewport is located at z = zLocation
	Line3d Scene::getRayFromScreen(int x, int y) // x and y represents indices in pixelgrid
	{
		float stepSizeX = viewPortWidth / (float)width;
		float stepSizeY = viewPortHeight / (float)height;
		
		// Center of viewport is located in (0,0,0)
		float startingX = -(viewPortWidth / 2.0f) + (stepSizeX / 2.0f);
		float startingY = (viewPortHeight / 2.0f) - (stepSizeX / 2.0f);
		float newX = startingX + x * stepSizeX;
		float newY = startingY + y * stepSizeY;

		// Wrap into Line3d-instance
		Vector3d direction = Vector3d(newX, newY, zLocation);
		Line3d ray = Line3d(Vector3d(), direction);
		return ray;

	}

	CollisionObject Scene::findClosestObject(Line3d ray)
	{
		CollisionObject* collision = &CollisionObject();
		float previousDistance = 1000000; // TODO infinity

		for (vector<Object3d*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
			Object3d* object = *it;
			Vector3d hit = object->CalculateCollisionPosition(ray);
			if (hit.length > 0 && hit.length < previousDistance) // can only do this since the start point is placed at (0,0,0) NEED TO FIX THIS for recoursion
			{
				collision = &CollisionObject(object, hit);
			}
		}
		return *collision;
	}

	Color^ Scene::rayTrace(Line3d ray)
	{
		Color^ outColor = backgroundColor();

		CollisionObject closestObject = findClosestObject(ray);
		if (closestObject.isReal)
		{
			// TODO shade the object
			outColor->alpha = 255;
			outColor->red = 200;
			outColor->green = 255;
			outColor->blue = 255;
		}
		// White color stub

		return outColor;
	}
}
