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
	// Helper method that determines corresponding x-index in a 2D-array, given index in 1D array
	int Scene::determineCorrespondingXIndex(int indexIn1DArray)
	{
		int index = (int)indexIn1DArray / width;
		return index;
	}

	// Helper method that determines corresponding y-index in a 2D-array, given index in 1D array
	int Scene::determineCorrespondingYIndex(int indexIn1DArray)
	{
		int index = (int)indexIn1DArray % width;
		return index;
	}

	array<Color^>^ Scene::render()
	{
		// This is where the magic happens: main-loop!
		std::cout << "Yolo world!";
		for (int i = 0; i < width * height; i++)
		{
			// Get ray
			int x = determineCorrespondingXIndex(i);
			int y = determineCorrespondingYIndex(i);
			Line3d ray = DetermineLine3dRepresentation(x, y);

			// Now, do something usefull with the ray...

			DetermineColorAtPixel(i);
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
	Line3d Scene::DetermineLine3dRepresentation(int x, int y) // x and y represents indices in pixelgrid
	{
		int stepSizeX = viewPortWidth / width;
		int stepSizeY = viewPortHeight / height;
		
		// Center of viewport is located in (0,0,0)
		int startingX = -(viewPortWidth / 2.0) + (stepSizeX / 2.0);
		int startingY = (viewPortHeight / 2.0) - (stepSizeX / 2.0);
		int x = startingX + x * stepSizeX;
		int y = startingY + y *stepSizeY;

		// Wrap into Line3d-instance
		Vector3d direction = Vector3d(x, y, zLocation);
		Vector3d startPoint = Vector3d(0.0, 0.0, 0.0);
		Line3d ray = Line3d(startPoint, direction);
		return ray;

	}

	void Scene::DetermineColorAtPixel(int pixelElement)
	{
		// Dummy-implmentation
		/*
		// First, determine x- and y-component corresponding to pixelElement
		int x = (int)pixelElement / width; // Floors decimal. 
		int y = pixelElement % width;

		Color^ dummyColor;	// Cyan-color
		dummyColor->alpha = 1;
		dummyColor->red = 75;
		dummyColor->green = 255;
		dummyColor->blue = 75;

		setColor(x, y, dummyColor);*/


		// Non-dummy implementation
		
		// First, determine x- and y-component corresponding to pixelElement
		int x = (int)pixelElement / width; // Floors decimal. 
		int y = pixelElement % width;

		// Get ray-representation
		Line3d ray = DetermineLine3dRepresentation(x, y);
	}
}
