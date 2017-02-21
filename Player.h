#pragma once
#include "Units.h"
#include <iostream>
using namespace std;

class Player : public Units//inheritance
{
private:

public:
	Player();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int copy_dir_x, int copy_dir_y, int copy_vel_x, int copy_vel_y);
	void Update(double cameraX, double cameraY);
	void Render();

	int GetHealth() { return Units::health; }

	void TakeDamage() { health--; }

	void StateHandler();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void ResetAnimation(int position);
	void Dash(double MouseAngle);
};