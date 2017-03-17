#pragma once
#include "Units.h"
#include <iostream>
#include <vector>
using namespace std;

class Cultist : public Units//inheritance
{
private:
	int health;
	int timer;
	bool TargetFound;
	vector<GameObject *>::iterator iter;
	GameObject *Target;
public:
	Cultist();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

	int GetHealth() { return health; }

	void TakeDamage() { health--; }

	void AnimationHandler();

	void Orbit(GameObject *Target);
};