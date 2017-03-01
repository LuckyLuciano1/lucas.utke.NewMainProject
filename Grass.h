#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
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

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double image_size_x, double image_size_y);
	void Update(double CameraX, double CameraY, vector<GameObject*> &objects);
	void Render();

};