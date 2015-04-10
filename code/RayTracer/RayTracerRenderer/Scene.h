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

	int getColor(int x, int y);
	void setColor(int x, int y, int color);
};

#endif /* SCENE_H */