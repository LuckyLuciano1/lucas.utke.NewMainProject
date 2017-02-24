#pragma once
#include "GameObject.h"
#include <vector>
#include <iostream>
using namespace std;

class Units : public GameObject//inheritance
{
private:
	//none here
protected:
	int health;//protected is so that obejetcs who inherit variables from Units can modify/access them
	int timer;
	enum ANIMATIONLIST {
		IDLELEFT, IDLERIGHT,
		MOVINGLEFT, MOVINGRIGHT,
		DASHLEFT, DASHRIGHT,
		PECKING
	};//list of all the various animations that can be taken for all units.
	int Animation;//tracks which Action is being performed by Units
public:

	Units();
	void Init(double x, double y, double velX, double velY, double dirX, double dirY, int boundX, int boundY, int ID, int TIER, int timer, int health);
	void Update(double CameraX, double CameraY, vector<GameObject*> &objects);
	void Render();
	void Destroy();

	int GetHealth() { return health; }
	void TakeDamage() { health--; }

	void Collided(GameObject *otherObject);
	
	void Pursue(GameObject *otherObject);
};