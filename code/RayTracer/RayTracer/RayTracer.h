// RayTracer.h

#pragma once

using namespace System;

namespace RayTracer {

	// This struct is declared ref, because it is used from C#-side
	public ref class Color 
	{
		public:
		int red, green, blue, alpha;
	};

	struct RayRepresentation
	{
		int x;
		int y;
		int z;
	};

	struct Point
	{	// Should be declared as floats instead
		int x;
		int y;
		int z;
	};

	struct Circle
	{
		int x;
		int y;
		int z;
		int radius;
	};

	public ref class Scene
	{
	public:
		Scene(int,int,int); // Constructor
		void init(const int width, const int height);
		void seedElementsIntoScene();
		array<Color^>^ render();
	private:
		int width;
		int height;
		int viewPortWidth, viewPortHeight, zLocation;
		array<Color^>^ arr; // int should be color

		Color^ getColor(int x, int y);
		void setColor(int x, int y, Color^ color);
		RayRepresentation DetermineRayRepresentation(int x, int y);
		Point* DetermineIntersection(RayRepresentation ray);
		void DetermineColorAtPixel(int);
	};
}
