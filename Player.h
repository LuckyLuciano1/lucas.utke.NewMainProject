#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class Player : public GameObject//inheritance
{
private:
	int Health;
	int Timer;
	enum ACTION {
		IDLELEFT, IDLERIGHT,
		MOVINGLEFT, MOVINGRIGHT,
		DASHLEFT, DASHRIGHT,
		CHARGELEFT, CHARGERIGHT,
		LUNGELEFT, LUNGERIGHT
	};//list of all the various animations that the player has
	int Animation;//tracks which animtaion is being performed by the main character
	int ChargeTime;//tracks amount that character has charged attack

public:
	Player();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, int ref_dir_x, int ref_dir_y, int ref_vel_x, int ref_vel_y);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

	int GetHealth() { return Health; }

	void TakeDamage() { Health--; }

	void AnimationHandler();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void ResetAnimation(int position);
	void Dash(double MouseAngle);
	void Charge(int mousex);//chrages attack
	int GetChargeTime() { return ChargeTime; }//returns how much attack is charged
	void SetChargeTime(int ChargeTime) { Player::ChargeTime = ChargeTime; }
	void Lunge(double MouseAngle);//attack itself
};