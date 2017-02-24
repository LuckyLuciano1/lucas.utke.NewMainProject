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
public:
	Cultist();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

	int GetHealth() { return health; }

	void TakeDamage() { health--; }

	void AnimationHandler();

	void Pursue(GameObject *otherObject);
};