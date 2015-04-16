// This is the main DLL file.

#include "stdafx.h"
#include <iostream>
#include <ctgmath>
#include <cmath>
#include <vector>

#include "Sphere3d.h"
#include "Scene.h"

using namespace std;

namespace RayTracer {

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
	int sx, sy;

	array<Color^>^ Scene::render()
	{
		// Things in the scene goes here for now
		sceneObjects = vector<Object3d*>(1);
		sceneObjects[0] = new Sphere3d(Vector3d(0,0,50),50);

		// This is where the magic happens: main-loop!
		for (int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++) {
				sx = x;
				sy = y;
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

	Color^ Scene::rayTrace(Line3d ray)
	{
		Color^ outColor = gcnew Color();
		float prev = 1000000; // TODO infinity

		outColor->alpha = 255;
		outColor->red = 255;
		outColor->green = sy%255;
		outColor->blue = sx%255;

		for(vector<Object3d*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
			Object3d* object = *it;

			Vector3d hit = object->CalculateCollisionPosition(ray);
			if(hit.length > 0 && hit.length < prev) { // Hit if not zero
				prev = hit.length;
				Vector3d normal = object->CalculateNormal(hit);
				
				// White color stub
				outColor->alpha = 255;
				outColor->red = 255;
				outColor->green = 255;
				outColor->blue = 255;
			}

		}

		return outColor;
	}
}
