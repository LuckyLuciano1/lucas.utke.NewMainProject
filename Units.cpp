#include "Units.h"

#define PI 3.14159265
#define DEGREES(x) int((x)/360.0*0xFFFFFF)
#define RADIANS(x) int((x)/2/M_PI*0xFFFFFF)

Units::Units() {
	timer = 0;
	health = 0;
}//the Units class includes movecommands and basic unit AI (attacking melee, ranged, responding to commands, etc)

 /*
 explanation of animation and how it works for units:
 curAnim (vertical) and curFrame (horizontal) both tell the position of the desired frame in the image, when mulitplied by frameHeihgt/frameWidth.
 curFrame increases until it hits maxFrame, at which point it resets (decided in Update)
 curAnim and maxFrame are decided by the Action variable, which keeps track of which ACTION state the unit is in. curAnim and maxFrame are decided within StateHandler.
 */
void Units::Init(double x, double y, double z, double velX, double velY, double dirX, double dirY, int boundX, int boundY, int ID, int TIER, int timer, int health)
{
	GameObject::Init(x, y, z, velX, velY, dirX, dirY, boundX, boundY, ID, TIER);
	Units::timer = timer;
	Units::health = health;
}
void Units::Update(double CameraX, double CameraY, vector<GameObject*> &objects)
{
	timer++;
	if (timer >= 30) {
		timer = 0;
		curFrame++;
		if (curFrame >= maxFrame)
			curFrame = 0;
	}
	GameObject::Update(CameraX, CameraY, objects);
}
void Units::Collided(GameObject *Target)
{
	GameObject::Collided(Target);
}
void Units::Render()	
{
	GameObject::Render();
}
void Units::Destroy()
{
	GameObject::Destroy();
}

void Units::Orbit(GameObject *Target) {
	double oX = Target->GetX();
	double oY = Target->GetY();
	int obX = Target->GetBoundX();
	int obY = Target->GetBoundY();
	//int hypotenuse = sqrt(((y - (oY + (obY / 2)))*(y - (oY + (obY / 2)))) + ((x - (oX + (obX / 2)))*(x - (oX + (obX / 2)))));//finds the length of the distance between object and target
	int hypotenuse = sqrt((y - oY)*(y - oY) + (x - oX)*(x - oX));//finds the length of the distance between object and target

	if (hypotenuse > 50) {//contracting spiral
		dirX = -sin(atan2((y - oY), (x - oX)) + 120);// the plus 90 adjusts the angle. without it, the unit will orbit the target.
		dirY = cos(atan2((y - oY), (x - oX)) + 120);
	}
	else if (hypotenuse < 45) {//expanding spiral
		dirX = sin(atan2((y - oY), (x - oX)) + 120);// the plus 90 adjusts the angle. without it, the unit will orbit the target.
		dirY = -cos(atan2((y - oY), (x - oX)) + 120);
	}
	else {//maintaining orbit
		dirX = -sin(atan2((y - oY), (x - oX)) + 0);// the plus 90 adjusts the angle. without it, the unit will orbit the target.
		dirY = cos(atan2((y - oY), (x - oX)) + 0);
	}

	if (dirX < 0)
		Animation = MOVINGLEFT;
	else
		Animation = MOVINGRIGHT;
}