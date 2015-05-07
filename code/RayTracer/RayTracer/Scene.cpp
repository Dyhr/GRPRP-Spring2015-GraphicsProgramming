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
#include "BoxSoftLight.h"
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

	Scene::Scene(int width, int height, float viewDistance)
	{
		this->width = width;						// Pixel-width
		this->height = height;						// Pixel-height
		this->viewPortWidth = 33;
		this->viewPortHeight = 23;
		this->stepSizeX = viewPortWidth / width;
		this->stepSizeY = viewPortHeight / height;

		this->zLocation = viewDistance;

		this->arr = gcnew array<Color^>(width*height);

		for(int i = 0; i < width*height; i++) 
		{
			this->arr[i] = gcnew Color;
		}
	}

	vector<Object3d*> sceneObjects;
	vector<LightBase*> lightObjects;
	vector<SoftLightbase*> softLightObjects;

	vector<ShaderBase*> shadersWhite;
	vector<ShaderBase*> shadersWhiteSpecular;
	vector<ShaderBase*> shadersRed;
	vector<ShaderBase*> shadersGreen;
	vector<ShaderBase*> shadersBlack;
	vector<ShaderBase*> shadersYellow;

	array<Color^>^ Scene::render()
	{
		sceneObjects = vector<Object3d*>();

		shadersWhite =			vector<ShaderBase*>();
		shadersWhiteSpecular =	vector<ShaderBase*>();
		shadersRed =			vector<ShaderBase*>();
		shadersGreen =			vector<ShaderBase*>();
		shadersBlack =			vector<ShaderBase*>();
		shadersYellow =			vector<ShaderBase*>();

		TwoSpheresInCornellBox();

		lightObjects = vector<LightBase*>(3);
		lightObjects[0] = new AmbientLight(0.15f);
		lightObjects[1] = new PositionalLight(0.75f, Point3d(0, 4.5f, 10), 15.0f, ColorIntern(255, 255, 255, 255));
		lightObjects[2] = new PositionalLight(0.2f, Point3d(-1, 0, -1), 5.0f, ColorIntern(255, 255, 255, 255));

		softLightObjects = vector<SoftLightbase*>(0);
		//softLightObjects[0] = new BoxSoftLight(0.75f, Point3d(0, 4.5f, 10), 10.0f, ColorIntern(255, 255, 255, 255), 1.0f, 0.1f, 1.0f, 11,1,10);

		// This is where the magic happens: main-loop!

	
		for (int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++) {
				Line3d ray = getRayFromScreen(x, y);
				ColorIntern color = rayTrace(ray,5,sceneRefractionIndex);	// TODO: 10 should be replaced by variable

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
		float px = -(viewPortWidth) / 2.0f + stepSizeX * x;
		float py = (viewPortHeight) / 2.0f - stepSizeY * y;

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
					collision = &CollisionObject(object, hit);
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
			outColor = ColorIntern(0, 0, 0, 255);
			Vector3d normal = closestObject.hit.normal;
			vector<LightBase*> lightsThatHit = getLightsThatHitPointSoftShadows(closestObject.hit.point); // shadows

			ColorIntern shadingColor = closestObject.object->shadeThis(ray.direction, normal, closestObject.hit.point, lightsThatHit);
			outColor = ColorIntern::blendAddition(shadingColor,outColor);
			if (count > 0)
			{
				Material materialOfObject = closestObject.object->material;

				float amtOfReflectance;
				if (materialOfObject.transparency > 0.0f ) // 
				{
					float nextRefraction;
					Vector3d newNormal = normal;
					if (Vector3d::dotProduct(normal, ray.direction) < 0.0f) // entering object
					{
						nextRefraction = materialOfObject.materialRefractionIndex;
					}
					else // leaving object
					{
						nextRefraction = sceneRefractionIndex;
						newNormal = Vector3d::negate(newNormal);
					}

					amtOfReflectance = Vector3d::reflectanceContributionOfRefraction(newNormal, ray.direction, currentRefractionIndex, nextRefraction);

					if (amtOfReflectance != 1.0f)
					{
						ColorIntern refractionContribution = getRefractionColor(newNormal, ray.direction, count, closestObject.hit.point, currentRefractionIndex, nextRefraction, materialOfObject);

						outColor = ColorIntern::blendByAmount(refractionContribution, outColor, materialOfObject.transparency - (materialOfObject.transparency*amtOfReflectance));
					}
				}

				if (materialOfObject.reflectiveness > 0.0f)
				{
					// Take reflection into account
					ColorIntern reflectionContribution = getReflectionColor(normal,ray.direction, count,closestObject.hit.point,currentRefractionIndex);

					outColor = ColorIntern::blendByAmount(reflectionContribution, outColor, materialOfObject.reflectiveness + (materialOfObject.transparency*amtOfReflectance));
				}
			}
		}
		return outColor;
	}

	ColorIntern Scene::getReflectionColor(Vector3d normal, Vector3d incomingDirection, int count, Point3d hitPoint, float currentRefractionIndex)
	{
		// Get reflection ray
		Vector3d vectorForOutgoingReflectedRay = Vector3d::reflectionVector(normal, incomingDirection);
		Point3d pointForOutgoingReflectedRay = hitPoint;
		Line3d reflectedRay = Line3d(pointForOutgoingReflectedRay, vectorForOutgoingReflectedRay).pushStartAlongLine(0.001f);

		ColorIntern reflectionContribution = rayTrace(reflectedRay, count - 1, currentRefractionIndex);

		return reflectionContribution;
	}

	ColorIntern Scene::getRefractionColor(Vector3d newNormal, Vector3d incomingDirection, int count, Point3d hitPoint, float currentRefractionIndex, float nextRefraction, Material materialOfObject)
	{
		Vector3d vectorForRefractedRay = Vector3d::refractionVector(incomingDirection, newNormal, currentRefractionIndex, nextRefraction);
		if (vectorForRefractedRay.length != 0.0f)
		{

			Point3d pointForOutgoingRefractedRay = hitPoint;
			Line3d refractedRay = Line3d(pointForOutgoingRefractedRay, vectorForRefractedRay).pushStartAlongLine(0.0001f);

			ColorIntern refractionContribution = rayTrace(refractedRay, count - 1, nextRefraction);
			return refractionContribution;
		}
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
					if (object->material.transparency < 0.5f) // in a better solution - intensity of light is decreased based on transparrency.
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
				}
				if (!isIntercepted)
				{
					lightsThatHit.push_back(light);
				}
			}
		}
		//TODO: Refactor inner loop into own method.
		for each (SoftLightbase* softLight in softLightObjects)
		{
			for each (LightBase* light in softLight->getLights())
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
						if (object->material.transparency < 0.5f) // in a better solution - intensity of light is decreased based on transparrency.
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
					}
					if (!isIntercepted)
					{
						lightsThatHit.push_back(light);
					}
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


	vector<LightBase*> Scene::getLightsThatHitPointSoftShadows(Point3d point)
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
				vector<Line3d> rays = ray.getTwistedLines(amtOfShadowRays, 0.5f);

				float newIntensity = 1.0f;
				for each (Object3d* object in sceneObjects)
				{
					for each (Line3d shadowRay in rays)
					{
						RayHit hit = object->CalculateCollision(shadowRay.pushStartAlongLine(0.001f));
						if (hit.success)
						{
							// this fix only works as long as we dont normalize the getLightOnPoint in positionalLights
							if (light->getLightType() == POSITIONAL && Vector3d(point, hit.point).length > light->GetLightOnPoint(point).length)
							{
							}
							else
							{
								newIntensity -= ((1.0f / sceneObjects.size()));//* object->material.transparency);
							}
						}
					}
				}
				if (newIntensity>= 0.00001f)
				{
					lightsThatHit.push_back(light->getCopyOfLight(newIntensity));
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

	void Scene::initLists()
	{

	}

	void Scene::setUpCornellBox()
	{
		shadersWhite.push_back(new AmbientShader(ColorIntern(255, 240, 245, 255)));
		shadersWhite.push_back(new DiffuseShader(ColorIntern(255, 240, 245, 255)));

		shadersRed.push_back(new AmbientShader(ColorIntern(235, 45, 20, 255)));
		shadersRed.push_back(new DiffuseShader(ColorIntern(235, 45, 20, 255)));

		shadersGreen.push_back(new AmbientShader(ColorIntern(30, 235, 55, 255)));
		shadersGreen.push_back(new DiffuseShader(ColorIntern(30, 235, 55, 255)));

		shadersBlack.push_back(new AmbientShader(ColorIntern(3, 3, 3, 255)));
		shadersBlack.push_back(new DiffuseShader(ColorIntern(3, 3, 3, 255)));

		shadersYellow.push_back(new AmbientShader(ColorIntern(235, 235, 55, 255)));
		shadersYellow.push_back(new DiffuseShader(ColorIntern(235, 235, 55, 255)));

		// Planes
		sceneObjects.push_back(new Plane3d(Point3d(0, -3, 0), Vector3d(0, 1, 0), shadersWhite));
		sceneObjects.push_back(new Plane3d(Point3d(0, 0, 15), Vector3d(0, 0, -1), shadersYellow));
		sceneObjects.push_back(new Plane3d(Point3d(0, 5, 0), Vector3d(0, -1, 0), shadersWhite));
		sceneObjects.push_back(new Plane3d(Point3d(-5, 0, 0), Vector3d(1, 0, 0), shadersRed));
		sceneObjects.push_back(new Plane3d(Point3d(5, 0, 0), Vector3d(-1, 0, 0), shadersGreen));
		sceneObjects.push_back(new Plane3d(Point3d(0, 0, -5), Vector3d(0, 0, 1), shadersBlack));
	}

	void Scene::TwoSpheresInCornellBox()
	{
		setUpCornellBox();
		shadersWhiteSpecular.push_back(new AmbientShader(ColorIntern(255, 240, 245, 255)));
		shadersWhiteSpecular.push_back(new DiffuseShader(ColorIntern(255, 240, 245, 255)));
		shadersWhiteSpecular.push_back(new SpecularShader(ColorIntern(250, 250, 255, 255),10.0f));

		sceneObjects.push_back(new Sphere3d(Point3d(-1, -2, 8), 1, shadersWhiteSpecular, Material(0.0f, 0.0f, 1.03f)));
		sceneObjects.push_back(new Sphere3d(Point3d(2, -1, 9), 2, shadersWhiteSpecular, Material(0.0f, 0.0f, 0.95f)));
	
	}
}
