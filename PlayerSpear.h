#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class PlayerSpear : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;
	enum SpearType { SPINNING, LUNGING, STABLE };
	int SpearState;
	int color;

public:
	PlayerSpear();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double image_x, double image_y, bool stable);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

};