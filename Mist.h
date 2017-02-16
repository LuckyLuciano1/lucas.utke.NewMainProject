#pragma once
#include "GameObject.h"
#include <iostream>
using namespace std;

class Mist : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;
	double RandDirX;
	double RandDirY;
	int RandSize;
	int MistID;

public:
	Mist();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int copy_MistID);
	void Update(double cameraX, double cameraY);
	void Render();

};