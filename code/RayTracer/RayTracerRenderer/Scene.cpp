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