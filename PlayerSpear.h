#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class PlayerSpear : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;
	enum SpearType { SPINNING, LUNGING, STABLE };
	int SpearState;
	double SpearAngle;
	double SpearTipX;
	double SpearTipY;
	ALLEGRO_BITMAP *ColorImage;

public:
	PlayerSpear();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *ColorImage, double ref_x, double ref_y, double ref_SpearAngle);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

	int GetState() { return SpearState; }
	void SetState(int SpearState) { PlayerSpear::SpearState = SpearState; }
	int GetAngle() { return SpearAngle; }
	void SetAngle(int SpearAngle) { PlayerSpear::SpearAngle = SpearAngle; }

};