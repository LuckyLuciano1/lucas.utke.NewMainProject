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

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, int image_size_x, int image_size_y, int ref_dir_x, int ref_dir_y, int ref_vel_x, int ref_vel_y, int ref_health);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

	int GetHealth() { return health; }

	void TakeDamage() { health--; }

	void StateHandler();
};