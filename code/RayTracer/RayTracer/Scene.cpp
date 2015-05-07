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
		ColorIntern background = ColorIntern(0, 0, 0, 255);
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
		this->viewPortWidth = 35;
		this->viewPortHeight = 25;
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
		initLists();
		WindSetup();

		//lonelyPlane();
		srand(time(NULL));

		// This is where the magic happens: main-loop!
		for (int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++) {
				Line3d ray = getRayFromScreen(x, y);
				ColorIntern color = rayTrace(ray,3,sceneRefractionIndex);	// TODO: 10 should be replaced by variable

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
			vector<LightBase*> lightsThatHit;
			if (shadowsOn)
			{
				lightsThatHit = getLightsThatHitPointSoftShadows(closestObject.hit.point); // shadows
			}else
			{
				lightsThatHit = lightObjects;
			}

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
		return ColorIntern(0,0,0,255);
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
				vector<Line3d> rays = ray.getTwistedLines(amtOfShadowRays, softShadowSpread);

				float newIntensity = 1.0f;
				for each (Object3d* object in sceneObjects)
				{
					if (object->objectType() == NONPLANE)
					{
						for each (Line3d shadowRay in rays)
						{
							RayHit hit = object->CalculateCollision(shadowRay.pushStartAlongLine(0.01f));
							if (hit.success)
							{
								// this fix only works as long as we dont normalize the getLightOnPoint in positionalLights
								if (light->getLightType() == POSITIONAL && Vector3d(point, hit.point).length > light->GetLightOnPoint(point).length)
								{
								}
								else
								{
									//float cos = Vector3d::cosineToAngle(shadowRay.direction, ray.direction);
									newIntensity -= (1.0f / (float)sceneObjects.size()) * (1.0f - object->material.transparency);//* object->material.transparency);
								}
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
		sceneObjects = vector<Object3d*>();
		shadersWhite = vector<ShaderBase*>();
		shadersWhiteSpecular = vector<ShaderBase*>();
		shadersRed = vector<ShaderBase*>();
		shadersGreen = vector<ShaderBase*>();
		shadersBlack = vector<ShaderBase*>();
		shadersYellow = vector<ShaderBase*>();

		lightObjects = vector<LightBase*>();
	}

	void Scene::setUpCornellBox()
	{	// Planes
		sceneObjects.push_back(new Plane3d(Point3d(0, -3, 0), Vector3d(0, 1, 0), shadersWhite));
		sceneObjects.push_back(new Plane3d(Point3d(0, 0, 15), Vector3d(0, 0, -1), shadersYellow));
		sceneObjects.push_back(new Plane3d(Point3d(0, 5, 0), Vector3d(0, -1, 0), shadersWhite));
		sceneObjects.push_back(new Plane3d(Point3d(-5, 0, 0), Vector3d(1, 0, 0), shadersRed));
		sceneObjects.push_back(new Plane3d(Point3d(5, 0, 0), Vector3d(-1, 0, 0), shadersGreen));
		sceneObjects.push_back(new Plane3d(Point3d(0, 0, -5), Vector3d(0, 0, 1), shadersBlack));

		positionalAndAmbientLight();
	}

	void Scene::TwoSpheresInCornellBox()
	{
		setUpCornellBox();
		shadersWhiteSpecular.push_back(new AmbientShader(ColorIntern(255, 240, 245, 255)));
		shadersWhiteSpecular.push_back(new DiffuseShader(ColorIntern(255, 240, 245, 255)));
		shadersWhiteSpecular.push_back(new SpecularShader(ColorIntern(250, 250, 255, 255),10.0f));

		sceneObjects.push_back(new Sphere3d(Point3d(-1, -2, 8), 1, shadersWhiteSpecular,	 Material(1.0f, 0.0f, 1.0f)));
		sceneObjects.push_back(new Sphere3d(Point3d(2, -1, 9), 2, shadersWhiteSpecular,		Material(0.0f, 1.0f, 1.02f)));
	}

	void Scene::falloffOnLight()
	{
		shadersWhite.push_back(new DiffuseShader(ColorIntern(255, 240, 245, 255)));

		sceneObjects.push_back(new Plane3d(Point3d(0, -3, 0), Vector3d(0, 1, 0), shadersWhite));

		lightObjects.push_back(new PositionalLight(0.75f, Point3d(0, 4.5f, 10), 15.0f, ColorIntern(255, 255, 255, 255)));
	}

	void Scene::positionalAndAmbientLight()
	{
		lightObjects.push_back(new AmbientLight(0.15f));
		lightObjects.push_back(new PositionalLight(0.75f, Point3d(0, 4.5f, 10), 15.0f, ColorIntern(255, 255, 255, 255)));
		lightObjects.push_back(new PositionalLight(0.2f, Point3d(-1, 0, -1), 5.0f, ColorIntern(255, 255, 255, 255)));
	}

	void Scene::lonelySphere()
	{
		shadersWhite.push_back(new AmbientShader(ColorIntern(255, 255, 255, 255)));

		sceneObjects.push_back(new Sphere3d(Point3d(0, 0, 4), 1, shadersWhite, Material(0.0f, 0.0f, 1.0f)));
		lightObjects.push_back(new AmbientLight(0.75f));
	}

	void Scene::lonelyTriangle()
	{
		shadersWhite.push_back(new AmbientShader(ColorIntern(255, 255, 255, 255)));
		sceneObjects.push_back(new Triangle3d(Point3d(-1.0f, 0.0f, 8.0f), Point3d(2.0f, 0.0f, 8.0f), Point3d(0.0f, -1.0f, 3.0f), shadersWhite));
		lightObjects.push_back(new AmbientLight(0.75f));
	}

	void Scene::lonelyPlane()
	{
		shadersWhite.push_back(new AmbientShader(ColorIntern(255, 255, 255, 255)));
		sceneObjects.push_back(new Plane3d(Point3d(0, -3, 0), Vector3d(0, 1, 0), shadersWhite));
		lightObjects.push_back(new AmbientLight(0.75f));
	}

	void Scene::lonelyNonReflectingSphereOnTopOfFloor()
	{
		lonelySphere();
		falloffOnLight();
	}

	void Scene::SphereWithAmbientShaderOnly()
	{
		shadersWhite.push_back(new AmbientShader(ColorIntern(255, 20, 20, 255)));
		sceneObjects.push_back(new Sphere3d(Point3d(0, 0, 4), 1, shadersWhite, Material(0.0f, 0.0f, 1.0f)));
		lightObjects.push_back(new AmbientLight(0.15f));
	}

	void Scene::SphereWithDiffuseShaderOnly()
	{
		shadersWhite.push_back(new DiffuseShader(ColorIntern(235, 45, 20, 255)));
		sceneObjects.push_back(new Sphere3d(Point3d(0, 0, 4), 1, shadersWhite, Material(0.0f, 0.0f, 1.0f)));
		lightObjects.push_back(new PositionalLight(1.00, Point3d(0, 6.0f, 0), 20.0f, ColorIntern(255, 255, 255, 255)));
	}

	void Scene::SphereWithSpecularShaderOnly()
	{
		shadersWhite.push_back(new SpecularShader(ColorIntern(240, 240, 240, 255), 10));
		sceneObjects.push_back(new Sphere3d(Point3d(0, 0, 4), 1, shadersWhite, Material(0.0f, 0.0f, 1.0f)));
		lightObjects.push_back(new PositionalLight(1.00, Point3d(2, 6.0f, 0), 20.0f, ColorIntern(255, 255, 255, 255)));
	}

	void Scene::SphereWithAmbientDiffuseAndSpecularShader()
	{
		shadersWhite.push_back(new AmbientShader(ColorIntern(255, 20, 20, 255)));
		shadersWhite.push_back(new DiffuseShader(ColorIntern(235, 45, 20, 255)));
		shadersWhite.push_back(new SpecularShader(ColorIntern(240, 240, 240, 255), 10));

		sceneObjects.push_back(new Sphere3d(Point3d(0, 0, 4), 1, shadersWhite, Material(0.0f, 0.0f, 1.0f)));

		lightObjects.push_back(new PositionalLight(0.75f, Point3d(2, 6.0f, 0), 20.0f, ColorIntern(255, 255, 255, 255)));
		lightObjects.push_back(new AmbientLight(0.15f));
	}
	

	void Scene::setUpShaders()
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
	}

	void Scene::WindSetup()
	{
		setUpShaders();
		setUpCornellBox();
		shadersWhiteSpecular.push_back(new AmbientShader(ColorIntern(255, 240, 245, 255)));
		shadersWhiteSpecular.push_back(new DiffuseShader(ColorIntern(255, 240, 245, 255)));
		shadersWhiteSpecular.push_back(new SpecularShader(ColorIntern(250, 250, 255, 255), 10.0f));

		sceneObjects.push_back(new Sphere3d(Point3d(-1, -1, 9), 1, shadersWhiteSpecular, Material(0.0f, 0.0f, 1.03f)));
		sceneObjects.push_back(new Sphere3d(Point3d(2, -1, 9), 2, shadersWhiteSpecular, Material(0.0f, 0.0f, 0.95f)));

	}
	void Scene::TrianglesInCornellBox() 
	{
		setUpCornellBox();

		sceneObjects.push_back(new Triangle3d(Point3d(-3, -2, 8), Point3d(-3, 2, 12), Point3d(-1, -2, 8), shadersRed, Material(0.0f, 0.0f, 0.95f)));

		vector<Triangle3d*> triangles = vector<Triangle3d*>(0);
		triangles.push_back(new Triangle3d(Point3d(-1, -3, 9), Point3d(-1, 2, 12), Point3d(2, -2, 8), shadersRed, Material(0.0f, 0.0f, 0.95f)));
		triangles.push_back(new Triangle3d(Point3d(-1, 2, 12), Point3d(3, 1, 13), Point3d(2, -2, 8), shadersRed, Material(0.0f, 0.0f, 0.95f)));
		sceneObjects.push_back(new Mesh3d(Point3d(), triangles, shadersGreen, Material(0.0f, 0.0f, 0.95f)));
	}
	void Scene::MeshInCornellBox() 
	{
		setUpCornellBox();

		vector< unsigned int > vertexIndices, normalIndices;
		vector< Point3d > temp_vertices;
		vector< Vector3d > temp_normals;
		vector< Triangle3d* > triangles;

		FILE* file = fopen("d.obj", "r");
		if(file == NULL) {
			printf("Impossible to open the file d.obj !\n");
			return;
		}
		while(true) {
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if(res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader
			if(strcmp(lineHeader, "v") == 0) {
				Point3d vertex = Point3d();
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			} else if(strcmp(lineHeader, "vn") == 0) {
				Vector3d normal = Vector3d();
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			} else if(strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if(matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

				triangles.push_back(new Triangle3d(temp_vertices[vertexIndex[0] - 1], temp_vertices[vertexIndex[1] - 1], temp_vertices[vertexIndex[2] - 1], shadersBlack, Material(0.0f, 0.0f, 1.0f)));
			}
		}

		sceneObjects.push_back(new Mesh3d(Point3d(0,0,8), triangles, shadersRed, Material(0.0f, 0.0f, 0.95f)));
	}
}
