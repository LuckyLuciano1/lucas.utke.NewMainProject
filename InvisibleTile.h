#pragma once
#include "GameObject.h"
#include <iostream>
using namespace std;

class InvisibleTile : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;

public:
	InvisibleTile();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, double image_x, double image_y, double image_size_x, double image_size_y, int TIER);
	void Update(double cameraX, double cameraY);
	void Render();

};