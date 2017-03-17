#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class InvisibleTile : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;

public:
	InvisibleTile();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, double image_x, double image_y, double image_size_x, double image_size_y, int TIER);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

};