#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class Dust : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;
	double RandDirX;
	double RandDirY;
	int RandSize;

public:
	Dust();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, int ref_SpawnSize_x, int ref_SpawnSize_y);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

};