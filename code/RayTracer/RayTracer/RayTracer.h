// RayTracer.h

#pragma once

using namespace System;

namespace RayTracer {
	public ref struct Color {
		int r;
	};

	public ref class Scene
	{
	public:
		Scene();
		void init(const int width, const int height);
		void render();
	private:
		int width;
		int height;
		int* arr; // int should be color

		int getColor(int x, int y);
		void setColor(int x, int y, int color);
	};
}
