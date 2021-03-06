#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

class MistSpawner : public GameObject//inheritance
{
private:

	int frameCounter;
	int TimeUp;
	int MistID;
	bool TargetFound;
	vector<GameObject *>::iterator iter;
	GameObject *Target;
public:
	MistSpawner();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, int ref_MistID);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();
	void Orbit(GameObject *Target);
};