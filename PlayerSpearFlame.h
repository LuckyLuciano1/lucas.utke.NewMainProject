#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class PlayerSpearFlame : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;
	double RandDirX;
	double RandDirY;
	int RandSize;
	int Multiplier;//intensity and size of flame is changed based on multiplier (will be ChargeTime of player)

public:
	PlayerSpearFlame();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, int ref_PlayerSpearFlameID);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

};