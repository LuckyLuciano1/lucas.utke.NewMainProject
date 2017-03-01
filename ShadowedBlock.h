#pragma once
#include "GameObject.h"
#include <vector>
#include <iostream>
using namespace std;

class ShadowedBlock : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;

public:
	ShadowedBlock();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double image_size_x, double image_size_y);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

};