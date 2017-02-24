#pragma once
#include "Units.h"
#include <iostream>
#include <vector>
using namespace std;

class Bird : public Units//inheritance
{
private:
	enum ANIM { IDLE, FLYING, PECKING };

public:
	Bird();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int image_size_x, int image_size_y, int copy_dir_x, int copy_dir_y, int copy_vel_x, int copy_vel_y, int copy_health);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

	int GetHealth() { return health; }

	void TakeDamage() { health--; }

	void StateHandler();
};