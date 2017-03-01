#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class Player : public GameObject//inheritance
{
private:
	int health;
	int timer;
	enum ACTION {
		IDLELEFT, IDLERIGHT,
		MOVINGLEFT, MOVINGRIGHT,
		DASHLEFT, DASHRIGHT
	};//list of all the various animations that the player has
	int Action;//tracks which Action is being performed by Units
public:
	Player();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, int ref_dir_x, int ref_dir_y, int ref_vel_x, int ref_vel_y);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

	int GetHealth() { return health; }

	void TakeDamage() { health--; }

	void AnimationHandler();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void ResetAnimation(int position);
	void Dash(double MouseAngle);
};