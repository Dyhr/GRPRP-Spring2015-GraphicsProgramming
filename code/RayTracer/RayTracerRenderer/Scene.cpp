#include <iostream>

#define amountOfPixelsHeight 700;
#define amountOfPixelsWidth 800;

class Scene
{
public:
	Scene();
	int createArray();
}

Scene::createArray()
{
	int returnValue [amountOfPixelsWidth][amountOfPixelsHeight] = {};
	return returnValue;
}

Scene::Scene()
{
}

Scene::~Scene()
{
}

int main()
{
	std::cout << "Hello world";
}