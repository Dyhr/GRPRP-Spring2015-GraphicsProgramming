// RayTracer.h

#pragma once

using namespace System;

namespace RayTracer {
	public ref struct Color {
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
		array<Color^>^ render();
	private:
		int width;
		int height;
		int viewPortWidth, viewPortHeight, zLocation;
		array<Color^>^ arr; // int should be color

		Color^ getColor(int x, int y);
		void setColor(int x, int y, Color^ color);
		RayRepresentation DetermineRayRepresentation(int x, int y);
		Point DetermineIntersection(RayRepresentation ray);
	};
}
