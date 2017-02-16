#pragma once
#include "GameObject.h"
#include <iostream>
using namespace std;

class MistSpawner : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;
	int frameCounter;
public:
	MistSpawner();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, double image_x, double image_y, double image_size_x, double image_size_y, bool collision);
	void Update(double cameraX, double cameraY);
	void Render();

};