#pragma once
#include "Units.h"
#include <iostream>
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
	void Update(double cameraX, double cameraY);
	void Render();

	int GetHealth() { return health; }

	void TakeDamage() { health--; }

	void AnimationHandler();

	void Pursue(GameObject *otherObject);
};