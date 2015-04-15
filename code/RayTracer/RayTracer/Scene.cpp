// This is the main DLL file.

#include "stdafx.h"
#include <iostream>
#include <ctgmath>
#include <cmath>

#define zDepth (10);

#include "Scene.h"

namespace RayTracer {

	Scene::Scene(int viewportWidth, int viewPortHeight, int zLocation) 
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

	array<Color^>^ Scene::render()
	{
		// This is where the magic happens: main-loop!
		std::cout << "Yolo world!";
		for (int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++) {
				Line3d ray = getRayFromScreen(x, y);
				Color^ color = rayTrace(ray);
				setColor(x, y, color);
			}
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
		int stepSizeX = viewPortWidth / width;
		int stepSizeY = viewPortHeight / height;
		
		// Center of viewport is located in (0,0,0)
		int startingX = -(viewPortWidth / 2.0) + (stepSizeX / 2.0);
		int startingY = (viewPortHeight / 2.0) - (stepSizeX / 2.0);
		int newX = startingX + x * stepSizeX;
		int newY = startingY + y *stepSizeY;

		// Wrap into Line3d-instance
		Vector3d direction = Vector3d(newX, newY, zLocation);
		Vector3d startPoint = Vector3d(0.0, 0.0, 0.0);
		Line3d ray = Line3d(startPoint, direction);
		return ray;

	}

	Color^ Scene::rayTrace(Line3d ray)
	{
		// Dummy-implentation

		Color^ dummyColor;	// Cyan-color
		dummyColor->alpha = 1;
		dummyColor->red = 75;
		dummyColor->green = 255;
		dummyColor->blue = 75;

		return dummyColor;
	}
}
