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
#include "PositionalLight.h"
#include "SpecularShader.h"
#include "Plane3d.h"
#include "Triangle3d.h"
#include "Mesh3d.h"


using namespace std;

namespace RayTracer {

	ColorIntern Scene::backgroundColor(){
		ColorIntern background = ColorIntern(10, 10, 10, 255);
		return background;
	}

	ColorIntern Scene::ambientColorOnObjects()
	{
		ColorIntern ambientColor = ColorIntern(200, 200, 200, 255);
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
	vector<ShaderBase*> shadersWhite;
	vector<ShaderBase*> shadersWhiteSpecular;
	vector<ShaderBase*> shadersRed;
	vector<ShaderBase*> shadersGreen;

	array<Color^>^ Scene::render()
	{
		sceneObjects = vector<Object3d*>(8);

		shadersWhite = vector<ShaderBase*>(2);
		shadersWhiteSpecular = vector<ShaderBase*>(3);
		shadersRed = vector<ShaderBase*>(2);
		shadersGreen = vector<ShaderBase*>(2);

		shadersWhite[0] = new AmbientShader(ColorIntern(255, 240, 245, 255));
		shadersWhite[1] = new DiffuseShader(ColorIntern(255, 240, 245, 255));

		shadersWhiteSpecular[0] = new AmbientShader(ColorIntern(255, 240, 245, 255));
		shadersWhiteSpecular[1] = new DiffuseShader(ColorIntern(255, 240, 245, 255));
		shadersWhiteSpecular[2] = new SpecularShader(ColorIntern(255, 255, 255, 255),5.0f);

		shadersRed[0] = new AmbientShader(ColorIntern(235, 45, 20, 255));
		shadersRed[1] = new DiffuseShader(ColorIntern(235, 45, 20, 255));

		shadersGreen[0] = new AmbientShader(ColorIntern(30, 235, 55, 255));
		shadersGreen[1] = new DiffuseShader(ColorIntern(30, 235, 55, 255));
		
		// planes
		sceneObjects[0] = new Plane3d(Point3d(0, -3, 0), Vector3d(0, 1, 0), shadersWhite);
		sceneObjects[1] = new Plane3d(Point3d(0, 0, 15), Vector3d(0, 0, -1), shadersWhite);
		sceneObjects[2] = new Plane3d(Point3d(0, 5, 0), Vector3d(0, -1, 0), shadersWhite);
		sceneObjects[3] = new Plane3d(Point3d(-5, 0, 0), Vector3d(1, 0, 0), shadersRed);
		sceneObjects[4] = new Plane3d(Point3d(5, 0, 0), Vector3d(-1, 0, 0), shadersGreen);

		sceneObjects[5] = new Sphere3d(Point3d(-1, -2, 8), 1, shadersWhiteSpecular, Material());
		sceneObjects[6] = new Sphere3d(Point3d(2, -1, 12), 2, shadersWhiteSpecular, Material(0.4f));

		vector<Triangle3d*> meshTris = vector<Triangle3d*>(3);
		meshTris[0] = new Triangle3d(Point3d(-5, -2, 12), Point3d(0, -1, 12), Point3d(-5, 2, 10), shadersWhiteSpecular);
		meshTris[1] = new Triangle3d(Point3d(-1, -1, 8), Point3d(0, -1, 12), Point3d(5, -2, 10), shadersWhiteSpecular);
		meshTris[2] = new Triangle3d(Point3d(-1, -1, 8), Point3d(0, -1, 12), Point3d(-5, 2, 10), shadersWhiteSpecular);
		sceneObjects[7] = new Mesh3d(Point3d(),meshTris,shadersWhiteSpecular, Material(0.1f));

		//sceneObjects[4] = new Triangle3d(Point3d(-6, 3.8f, 10), Point3d(0, 4.5f, 12), Point3d(-2, 2.2f, 5), shadersOnObject2);

		lightObjects = vector<LightBase*>(5);
		lightObjects[0] = new AmbientLight(0.15f);
		//lightObjects[1] = new DirectionalLight(0.3f, Vector3d(0.5f, -1, 0.3f), ColorIntern(255,150,180,255));
		//lightObjects[2] = new DirectionalLight(0.5f, Vector3d(0.0f, -0.1f, 1.0f), ColorIntern(180, 150, 255, 255));
		lightObjects[1] = new PositionalLight(0.75f, Point3d(0, 4, 10), 10.0f, ColorIntern(255,230,230,255));
		lightObjects[2] = new PositionalLight(0.2f, Point3d(0, 0, 0), 10.0f, ColorIntern(255, 255, 255, 255));

		lightObjects[3] = new PositionalLight(0.2f, Point3d(-4.0f, 0, 10), 2.0f, ColorIntern(235, 45, 20, 255));
		lightObjects[4] = new PositionalLight(0.2f, Point3d(4.5f, 0, 10), 2.0f, ColorIntern(30, 235, 55, 255));

		// This is where the magic happens: main-loop!
		for (int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++) {
				Line3d ray = getRayFromScreen(x, y);
				ColorIntern color = rayTrace(ray,2,sceneRefractionIndex);	// TODO: 10 should be replaced by variable

				Color^ outColor = gcnew Color(color); // convert to outgoing color
				setColor(x, y, outColor);
			}
		}

		// Garbage collection
		for(vector<Object3d*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
			delete *it;
		}
		// TODO more of this

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
		float px = -(viewPortWidth) / 2.0f + viewPortSize * x;
		float py = (viewPortHeight) / 2.0f - viewPortSize * y;

		return Line3d(Point3d(), Vector3d::normalize(Vector3d(px, py, zLocation)));
	}

	CollisionObject Scene::findClosestObject(Line3d ray)
	{
		CollisionObject* collision = &CollisionObject();
		float previousDistance = 1000000; // TODO infinity

		for (vector<Object3d*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
			Object3d* object = *it;
			RayHit hit = object->CalculateCollision(ray);
			if (hit.success)
			{
				float distanceFromRayStart = Vector3d(hit.point, ray.position).length;

				if (distanceFromRayStart > 0 && distanceFromRayStart < previousDistance) // can only do this since the start point is placed at (0,0,0) NEED TO FIX THIS for recoursion
				{
					previousDistance = distanceFromRayStart;
					collision = &CollisionObject(object, hit.point);
				}
			}
		}
		return *collision;
	}

	ColorIntern Scene::rayTrace(Line3d ray, int count, float currentRefractionIndex)
	{
		ColorIntern outColor = backgroundColor();

		CollisionObject closestObject = findClosestObject(ray);
		if (closestObject.isReal)
		{
			Vector3d normal = closestObject.object->CalculateNormal(closestObject.collisionCoord);
			vector<LightBase*> lightsThatHit = getLightsThatHitPoint(closestObject.collisionCoord); // shadows

			ColorIntern shadingColor = closestObject.object->shadeThis(ray.direction, normal, closestObject.collisionCoord, lightsThatHit);
			
			if (count > 0)
			{
				Material materialOfObject = closestObject.object->material;
				if (materialOfObject.reflectiveness > 0.0f)
				{
					// Take reflection into account
					// Get reflection ray
					Vector3d vectorForOutgoingReflectedRay = Vector3d::reflectionVector(normal, ray.direction);
					Point3d pointForOutgoingReflectedRay = closestObject.collisionCoord;
					Line3d reflectedRay = Line3d(pointForOutgoingReflectedRay, vectorForOutgoingReflectedRay).pushStartAlongLine(0.001f);

					ColorIntern reflectionContribution = rayTrace(reflectedRay, count - 1, currentRefractionIndex);

					shadingColor = ColorIntern::blendByAmount(reflectionContribution, shadingColor, materialOfObject.reflectiveness);

				}

				if (materialOfObject.transparency > 0.0f)
				{
					Vector3d newNormal = normal;
					float nextRefraction = materialOfObject.materialRefractionIndex;
					if (currentRefractionIndex != sceneRefractionIndex)
					{
						newNormal = Vector3d::negate(newNormal);
						nextRefraction = sceneRefractionIndex;
					}
					

					Vector3d vectorForRefractedRay = Vector3d::refractionVector(newNormal, Vector3d::negate(ray.direction), currentRefractionIndex, nextRefraction);
					Point3d pointForOutgoingRefractedRay = closestObject.collisionCoord;
					Line3d reflectedRay = Line3d(pointForOutgoingRefractedRay, vectorForRefractedRay).pushStartAlongLine(0.001f);

					ColorIntern refractionContribution = rayTrace(reflectedRay, count - 1, nextRefraction);

					shadingColor = ColorIntern::blendByAmount(refractionContribution, shadingColor, materialOfObject.transparency);

				}
			}
			
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
			}
			else
			{
				Line3d ray = Line3d(point, Vector3d::negate(light->GetLightOnPoint(point)));
				bool isIntercepted = false;
				for each (Object3d* object in sceneObjects)
				{
					RayHit hit = object->CalculateCollision(ray.pushStartAlongLine(0.001f));
					if (hit.success)
					{
						// this fix only works as long as we dont normalize the getLightOnPoint in positionalLights
						if (light->getLightType() == POSITIONAL && Vector3d(point, hit.point).length > light->GetLightOnPoint(point).length)
						{
						}
						else
						{
							isIntercepted = true;
							break;
						}
						
					}
				}
				if (!isIntercepted)
				{
					lightsThatHit.push_back(light);
				}
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
