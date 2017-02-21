#pragma once
#include "GameObject.h"
#include <iostream>
using namespace std;

class Units : public GameObject//inheritance
{
private:
	//none here
protected:
	int health;//protected is so that obejetcs who inherit variables from Units can modify/access them
	int timer;
	int TimeUp;
	enum ACTION {
		IDLELEFT, IDLERIGHT,
		MOVINGLEFT, MOVINGRIGHT,
		DASHLEFT, DASHRIGHT,
		PECKING
	};//list of all the various animations (+movements/actions) that can be taken for all units.
	int Action;//tracks which Action is being performed by Units
	int ActionTimer;//dictates how long it is before Action is changed
public:

	Units();
	void Init(double x, double y, double velX, double velY, double dirX, double dirY, int boundX, int boundY, int ID, int TIER, int timer, int health, int TimeUp);
	void Update(double CameraX, double CameraY);
	void Render();
	void Destroy();

	int GetHealth() { return health; }
	void TakeDamage() { health--; }

	void Collided(GameObject *otherObject);

	void MoveUp();
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void ResetAnimation(int position);
	void Dash(double Angle);
};