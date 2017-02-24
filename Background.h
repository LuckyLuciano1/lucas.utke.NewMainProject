#pragma once

#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class Background : public GameObject
{
private:

public:
	Background(ALLEGRO_BITMAP *image);
	void Destroy();

	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();
};