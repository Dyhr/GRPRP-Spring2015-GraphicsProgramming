#ifndef SCENE_H
#define SCENE_H

class Scene {
public:
	Scene();
	void init(const int width, const int height);
	void render();
private:
	int width;
	int height;
	int* arr; // int should be color
};

#endif /* SCENE_H */