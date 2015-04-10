#include "Scene.h"
#include <iostream>

Scene::Scene() {

}
void Scene::init(const int width, const int height) {
	this->width = width;
	this->height = height;
	this->arr = new int[width*height];
}
void Scene::render() {
}

int Scene::getColor(int x, int y) {
	return this->arr[x+y*width];
}
void Scene::setColor(int x, int y, int color) {
	this->arr[x + y*width] = color;
}