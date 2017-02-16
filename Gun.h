#pragma once
#include "Units.h"
#include <iostream>
using namespace std;

class Gun : public GameObject//inheritance
{
private:
	double MouseAngle;
	int TimeUp;
public:
	Gun();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int image_size_x, int image_size_y, double MouseAngle);
	void Update(double cameraX, double cameraY);
	void Render();
};