#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class CloudBase : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;
	enum CloudType { SHRINKING, GROWING, PERMASTABLE };
	int CloudState;
	int color;

public:
	CloudBase();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, double image_x, double image_y, bool stable);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

};