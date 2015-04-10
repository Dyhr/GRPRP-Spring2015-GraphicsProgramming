#include <iostream>
#include "Scene.h"

int main() {
	Scene* scene = new Scene();
	scene->init(800, 600);
	scene->render();

	int i;
	std::cin >> i;
	delete scene;

	return 0;
}