// This is the main DLL file.

#include "stdafx.h"
#include <iostream>
#include <ctgmath>
#include <cmath>
#include <vector>
#include <list>

#include "Sphere3d.h"
#include "CollisionObject.h"
#include "Scene.h"
#include "Color.h"
#include "AmbientShader.h"
#include "DiffuseShader.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "SpecularShader.h"

using namespace std;

namespace RayTracer {

	ColorIntern Scene::backgroundColor(){
		ColorIntern background = ColorIntern(10, 10, 10, 255);
		return background;
	}

	ColorIntern Scene::ambientColorOnObjects()
	{
		ColorIntern ambientColor = ColorIntern(40, 40, 40, 255);
		return ambientColor;
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
	vector<LightBase*> lightObjects;
	vector<ShaderBase*> shadersOnObject1;

	void Scene::initializeSceneObjects()
	{
		// Things in the scene goes here for now
		sceneObjects = vector<Object3d*>(1);
		shadersOnObject1 = vector<ShaderBase*>(2);

		//shadersOnObject1[0] = &AmbientShader(ambientColorOnObjects());
		shadersOnObject1[0] = &(DiffuseShader(ColorIntern(255, 0, 255, 255)));
		shadersOnObject1[1] = &SpecularShader(ColorIntern(230, 230, 230, 255), 2.0f);
		sceneObjects[0] = &Sphere3d(Point3d(0, 100, 100), 20, shadersOnObject1);

		lightObjects = vector<LightBase*>(2);
		lightObjects[0] = &AmbientLight(0.1f);
		lightObjects[1] = &DirectionalLight(0.8f, Vector3d(0, -2, -1));
	}

	array<Color^>^ Scene::render()
	{
		//initializeSceneObjects();

		sceneObjects = vector<Object3d*>(1);
		shadersOnObject1 = vector<ShaderBase*>(2);

		shadersOnObject1[0] = &AmbientShader(ambientColorOnObjects());
		shadersOnObject1[1] = &(DiffuseShader(ColorIntern(255, 0, 255, 255)));
		//shadersOnObject1[2] = &SpecularShader(ColorIntern(230, 230, 230, 255), 0.5f);
		sceneObjects[0] = new Sphere3d(Point3d(0, 100, 100), 20, shadersOnObject1);

		lightObjects = vector<LightBase*>(2);
		lightObjects[0] = &AmbientLight(0.1f);
		lightObjects[1] = &DirectionalLight(0.8f, Vector3d(0, -2, -1));

		// This is where the magic happens: main-loop!
		for (int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++) {
				Line3d ray = getRayFromScreen(x, y);
				ColorIntern color = rayTrace(ray);

				Color^ outColor = gcnew Color(color); // convert to outgoing color
				setColor(x, y, outColor);
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
		Line3d ray = Line3d(Point3d(), direction);
		return ray;

	}

	CollisionObject Scene::findClosestObject(Line3d ray)
	{
		CollisionObject* collision = &CollisionObject();
		float previousDistance = 1000000; // TODO infinity

		for (vector<Object3d*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
			Object3d* object = *it;
			Point3d hit = object->CalculateCollisionPosition(ray);
			float distanceFromRayStart = Vector3d(hit, ray.position).length;

			if (distanceFromRayStart > 0 && distanceFromRayStart < previousDistance) // can only do this since the start point is placed at (0,0,0) NEED TO FIX THIS for recoursion
			{
				collision = &CollisionObject(object, hit);
			}

		}
		return *collision;
	}

	ColorIntern Scene::rayTrace(Line3d ray)
	{
		ColorIntern outColor = backgroundColor();

		CollisionObject closestObject = findClosestObject(ray);
		if (closestObject.isReal)
		{
			Vector3d normal = closestObject.object->CalculateNormal(closestObject.collisionCoord);
			ColorIntern shadingColor = closestObject.object->shadeThis(ray.direction, normal, closestObject.collisionCoord, lightObjects);
			outColor = ColorIntern::blendAddition(outColor, shadingColor);
		}
		// White color stub

		return outColor;
	}

	vector<LightBase*> Scene::getLightsThatHitPoint(Point3d point)
	{
		list<LightBase*> lightsThatHit = list<LightBase*>();
		for each (LightBase* light in lightObjects)
		{
			
		}
		
		return vector<LightBase*>(0);
	}
}
