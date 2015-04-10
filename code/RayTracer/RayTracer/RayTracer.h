// RayTracer.h

#pragma once

using namespace System;

namespace RayTracer {
	public ref struct Color {
		int red, green, blue, alpha;
	};

	public ref class Scene
	{
	public:
		Scene();
		void init(const int width, const int height);
		array<Color^>^ render();
	private:
		int width;
		int height;
		array<Color^>^ arr; // int should be color

		Color^ getColor(int x, int y);
		void setColor(int x, int y, Color^ color);
	};
}
