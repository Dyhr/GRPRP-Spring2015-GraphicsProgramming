// This is the main DLL file.

#include "stdafx.h"
#include <iostream>
#include <ctgmath>
#include <cmath>

#include "RayTracer.h"

namespace RayTracer {

	Scene::Scene(int viewportWidth, int viewPortHeight, int zLocation) 
	{
		this->viewPortWidth = viewPortWidth;
		this->viewPortHeight = viewPortHeight;
		this->zLocation = zLocation;
	}
	void Scene::init(const int width, const int height) {
		this->width = width;
		this->height = height;
		this->arr = gcnew array<Color^>(width*height);

		for(int i = 0; i < width*height; i++) {
			this->arr[i] = gcnew Color;
		}
	}
	array<Color^>^ Scene::render() {
		std::cout << "Hello World!\n";
		return arr;
	}

	Color^ Scene::getColor(int x, int y) {
		return arr[x + y*width];
	}
	void Scene::setColor(int x, int y, Color^ color) {
		arr[x + y*width] = color;
	}

	// Assumes zero-based pixel-grid
	RayRepresentation Scene::DetermineRayRepresentation(int x, int y)
	{
		int stepSize = viewPortWidth / width;
		
		int endingX = x * stepSize - stepSize / 2;
		int endingY = y *stepSize - stepSize / 2;

		// Wrap into struct
		RayRepresentation returnStruct;
		returnStruct.x = endingX;
		returnStruct.y = endingY;
		returnStruct.z = zLocation;

		return returnStruct;
	}

	Point DetermineIntersection(RayRepresentation ray)
	{
		// Hardcoded sphere for now
		Circle firstCircle;
		firstCircle.radius = 2;
		firstCircle.x = 8;
		firstCircle.y = 2;
		firstCircle.z = 10;

		// Because we cannot use exact numbers, we set a threshold
		double threshold = 0.01;

		int aCoefficient = pow(ray.x, 2) + pow(ray.y, 2) + pow(ray.z, 2);
		int bCoefficient = -2 * ((ray.x*firstCircle.x) + (ray.y*firstCircle.y) + (ray.z*firstCircle.z));
		int cCoefficient = pow(firstCircle.x, 2) + pow(firstCircle.y, 2) + pow(firstCircle.z, 2) - pow(firstCircle.radius,2);

		double discriminant = pow(bCoefficient, 2) - 4.0*aCoefficient*cCoefficient;

		if (discriminant < 0.0)
		{
			// No intersection
		}
		else if (discriminant < threshold)
		{
			// Model only a single intersection
			int t = -bCoefficient / (2 * aCoefficient);

			Point intersection;

			intersection.x = t * ray.x;
			intersection.y = t * ray.y;
			intersection.z = t * ray.z;

			return intersection;

		}
		else
		{
			// Two intersections

			// First intersection
			Point firstIntersection;
			int t1 = (-bCoefficient + sqrt(discriminant))/(2*aCoefficient);

			firstIntersection.x = t1 * ray.x;
			firstIntersection.y = t1 * ray.y;
			firstIntersection.z = t1 * ray.z;

			
			// TODO: Figurwe out how to return a list of results
			// Second intersection
			/*Point secondIntersection;
			int t1 = (-bCoefficient - sqrt(discriminant)) / (2 * aCoefficient);

			secondIntersection.x = t1 * ray.x;
			secondIntersection.y = t1 * ray.y;
			secondIntersection.z = t1 * ray.z;*/

			return firstIntersection;
		}
	}
}
