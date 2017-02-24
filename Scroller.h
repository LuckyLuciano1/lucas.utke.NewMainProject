#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class Scroller : public GameObject
{
private:

public:
	Scroller(ALLEGRO_BITMAP *image);
	void Destroy();

	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();
};