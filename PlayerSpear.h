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
	double SpearAngleRadians;
	double SpearTipX;
	double SpearTipY;
	ALLEGRO_BITMAP *ColorImage;

	bool TargetFound;//variables to find and store player information
	vector<GameObject *>::iterator iter;
	GameObject *Target;

public:
	PlayerSpear();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *ColorImage, double ref_x, double ref_y, double ref_SpearAngleRadians);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

	int GetAngle() { return SpearAngleRadians; }
	void SetAngle(int SpearAngleRadians) { PlayerSpear::SpearAngleRadians = SpearAngleRadians; }

	int GetSpearState() { return SpearState; }
	void SetSpearState(int SpearState) { PlayerSpear::SpearState = SpearState; }

};