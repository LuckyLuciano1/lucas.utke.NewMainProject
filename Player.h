#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
using namespace std;

class Player : public GameObject//inheritance
{
private:
	ALLEGRO_BITMAP *ColorImage;
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
	int ChargeTime;//tracks amount that character has charged  (affects damage of lunge)
	int LungeTime;//travks time of Lunge (determines how lunge player lunges for)

	double MouseAngleRadians;
public:
	Player();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *ref_ColorImage, double ref_x, double ref_y, int ref_dir_x, int ref_dir_y, int ref_vel_x, int ref_vel_y);
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
	

	int GetChargeTime() { return ChargeTime; }//returns how much attack is charged
	void SetChargeTime(int ChargeTime) { Player::ChargeTime = ChargeTime; }
	void Charge(int mousex);//charges attack

	int GetLungeTime() { return LungeTime; }//returns how long lunge has been going on for
	void SetLungeTime(int LungeTime) { Player::LungeTime = LungeTime; }
	void Lunge(double MouseAngleRadians);//attack itself

	double GetMouseAngleRadians() { return MouseAngleRadians; }
	void SetMouseAngleRadians(double MouseAngleRadians) { Player::MouseAngleRadians = MouseAngleRadians; }
};