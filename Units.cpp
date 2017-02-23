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
void Units::Init(double x, double y, double velX, double velY, double dirX, double dirY, int boundX, int boundY, int ID, int TIER, int timer, int health)
{
	GameObject::Init(x, y, velX, velY, dirX, dirY, boundX, boundY, ID, TIER);
	Units::timer = timer;
	Units::health = health;
}
void Units::Update(double CameraX, double CameraY)
{
	timer++;
	if (timer >= 30) {
		timer = 0;
		curFrame++;
		if (curFrame >= maxFrame)
			curFrame = 0;
	}
	GameObject::Update(CameraX, CameraY);
}
void Units::Collided(GameObject *otherObject)
{
	GameObject::Collided(otherObject);
}
void Units::Render()	
{
	GameObject::Render();
}
void Units::Destroy()
{
	GameObject::Destroy();
}

void Units::Pursue(GameObject *otherObject) {
	double oX = otherObject->GetX();
	double oY = otherObject->GetY();

	dirX = -sin(atan2(y - oY, x - oX)+90);// the plus 90 adjusts the angle. without it, the unit will orbit the target.
	dirY = cos(atan2(y - oY, x - oX)+90);

	if (dirX < 0)
		Animation = MOVINGLEFT;
	else
		Animation = MOVINGRIGHT;
}