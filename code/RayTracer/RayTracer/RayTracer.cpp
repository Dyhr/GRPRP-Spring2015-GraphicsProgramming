// This is the main DLL file.

#include "stdafx.h"
#include <iostream>

#include "RayTracer.h"

namespace RayTracer {

	Scene::Scene() {

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
}
