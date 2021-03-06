#pragma once
#include "GameObject.h"
#include "Player.h"
#include <iostream>
#include <vector>
using namespace std;

class PlayerSpear : public GameObject//inheritance
{
private:

	int image_x;
	int image_y;
	enum SpearType { SPINNING, LUNGING, IDLE };
	int SpearState;
	double SpearAngleRadians;
	double SpearTipX;
	double SpearTipY;
	int ChargeTime;//stores how long player has charged attack for (immediately passed into PlayerSpearFlame for its Multiplier effect)
	ALLEGRO_BITMAP *ColorImage;
	Player *player;

public:
	PlayerSpear();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *ColorImage, double ref_x, double ref_y, double ref_z, double ref_SpearAngleRadians, int ref_SpearState, int ref_ChargeTime, Player *ref_player);
	void Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void Render();

	int GetAngle() { return SpearAngleRadians; }
	void SetAngle(double SpearAngleRadians) { PlayerSpear::SpearAngleRadians = SpearAngleRadians; }

	int GetSpearState() { return SpearState; }
	void SetSpearState(int SpearState) { PlayerSpear::SpearState = SpearState; }

	int GetChargeTime() { return ChargeTime; }//returns how much attack of player is charged
	void SetChargeTime(int ChargeTime) { PlayerSpear::ChargeTime = ChargeTime; }

};