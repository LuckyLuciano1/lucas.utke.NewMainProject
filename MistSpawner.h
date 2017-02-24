#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class MistSpawner : public GameObject//inheritance
{
private:

	int frameCounter;
	int TimeUp;
	int MistID;
	vector<GameObject *>::iterator iter;
public:
	MistSpawner();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int copy_MistID);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();
	void Pursue(GameObject *otherObject);
};