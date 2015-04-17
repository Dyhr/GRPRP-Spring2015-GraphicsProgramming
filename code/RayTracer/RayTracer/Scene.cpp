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
#include "Plane3d.h"

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

	Scene::Scene(int width, int height, float viewSize, float viewDistance)
	{
		this->width = width;
		this->height = height;
		this->viewPortWidth = width * viewSize;
		this->viewPortHeight = height * viewSize;
		this->viewPortSize = viewSize;
		this->zLocation = viewDistance;

		this->arr = gcnew array<Color^>(width*height);

		for(int i = 0; i < width*height; i++) {
			this->arr[i] = gcnew Color;
		}
	}

	vector<Object3d*> sceneObjects;
	vector<LightBase*> lightObjects;
	vector<ShaderBase*> shadersOnObject1;
	vector<ShaderBase*> shadersOnObject2;

	array<Color^>^ Scene::render()
	{
		sceneObjects = vector<Object3d*>(3);
		shadersOnObject1 = vector<ShaderBase*>(2);
		shadersOnObject2 = vector<ShaderBase*>(2);

		shadersOnObject1[0] = new AmbientShader(ambientColorOnObjects());
		shadersOnObject1[1] = new DiffuseShader(ColorIntern(255, 0, 255, 255));

		shadersOnObject2[0] = new AmbientShader(ambientColorOnObjects());
		shadersOnObject2[1] = new DiffuseShader(ColorIntern(0, 255, 255, 255));
		//shadersOnObject1[2] = &SpecularShader(ColorIntern(230, 230, 230, 255), 0.5f);
		sceneObjects[0] = new Sphere3d(Point3d(0, 0, 10), 1, shadersOnObject1);
		sceneObjects[1] = new Sphere3d(Point3d(0, 2.1f, 10), 1, shadersOnObject2);
		//sceneObjects[2] = new Sphere3d(Point3d(80, 120, 10), 20, shadersOnObject1);
		sceneObjects[2] = new Plane3d(Point3d(0,-1,0), Vector3d(0,1,0), shadersOnObject1);

		lightObjects = vector<LightBase*>(2);
		lightObjects[0] = &AmbientLight(0.1f);
		lightObjects[1] = &DirectionalLight(0.8f, Vector3d(0.5f, -1, 0.3f));

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
		// Center of viewport is located in (0,0,0)
		float px = -(width*viewPortSize) / 2.0f + viewPortSize * x;
		float py =  (width*viewPortSize) / 2.0f - viewPortSize * y;

		return Line3d(Point3d(), Vector3d::normalize(Vector3d(px, py, zLocation)));
	}

	CollisionObject Scene::findClosestObject(Line3d ray)
	{
		CollisionObject* collision = &CollisionObject();
		float previousDistance = 1000000; // TODO infinity

		for (vector<Object3d*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
			Object3d* object = *it;
			Point3d hit = object->CalculateCollisionPosition(ray);
			if (hit.x != 0 && hit.y != 0 && hit.z != 0)
			{
				float distanceFromRayStart = Vector3d(hit, ray.position).length;

				if (distanceFromRayStart > 0 && distanceFromRayStart < previousDistance) // can only do this since the start point is placed at (0,0,0) NEED TO FIX THIS for recoursion
				{
					previousDistance = distanceFromRayStart;
					collision = &CollisionObject(object, hit);
				}

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
			vector<LightBase*> lightsThatHit = getLightsThatHitPoint(closestObject.collisionCoord); // todo Use
			ColorIntern shadingColor = closestObject.object->shadeThis(ray.direction, normal, closestObject.collisionCoord, lightsThatHit);
			outColor = ColorIntern::blendAddition(outColor, shadingColor);
		}

		return outColor;
	}

	vector<LightBase*> Scene::getLightsThatHitPoint(Point3d point)
	{
		list<LightBase*> lightsThatHit = list<LightBase*>();
		for each (LightBase* light in lightObjects)
		{
			if (light->getLightType() == AMBIENT)
			{
				lightsThatHit.push_back(light);
				continue;
			}

			Line3d ray = Line3d(point, Vector3d::negate(light->GetLightOnPoint(point)));
			bool isIntercepted = false;
			for each (Object3d* object in sceneObjects)
			{
				Point3d hit = object->CalculateCollisionPosition(ray.pushStartAlongLine(0.001f));
				if (hit.x != 0 && hit.y != 0 && hit.z != 0)
				{
					isIntercepted = true;
					break;
				}
			}
			if (!isIntercepted)
			{
				lightsThatHit.push_back(light);
			}
		}

		// copy to vector 
		vector<LightBase*> lightsToReturn = vector<LightBase*>(lightsThatHit.size());
		int i = 0;
		for each (LightBase* light in lightsThatHit)
		{
			lightsToReturn[i++] = light;
		}
		return lightsToReturn;
	}
}
