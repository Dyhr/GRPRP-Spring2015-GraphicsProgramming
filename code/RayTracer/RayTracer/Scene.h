#pragma once
#include "Line3d.h"

using namespace System;

namespace RayTracer {

	// This struct is declared ref, because it is used from C#-side
	public ref class Color 
	{
		public:
		int red, green, blue, alpha;
	};

	public ref class Scene {
	public:
		Scene(int,int,int); // Constructor
		void init(const int width, const int height);
		array<Color^>^ render();
	private:
		int width;
		int height;
		int viewPortWidth, viewPortHeight, zLocation;
		array<Color^>^ arr;

		Color^ getColor(int x, int y);
		void setColor(int x, int y, Color^ color);
		Line3d getRayFromScreen(int x, int y);
		Color^ rayTrace(Line3d ray);
	};
}
