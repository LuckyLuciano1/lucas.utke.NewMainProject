#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class Gun : public GameObject//inheritance
{
private:
	double MouseAngle;
	int TimeUp;
public:
	Gun();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, int image_size_x, int image_size_y, double MouseAngle);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();
};