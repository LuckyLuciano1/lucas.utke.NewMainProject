#pragma once
#include "GameObject.h"
#include <iostream>
using namespace std;

class Grass : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;
	int timer;

public:
	Grass();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, double image_size_x, double image_size_y);
	void Update(double CameraX, double CameraY);
	void Render();

};