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
void Units::Init(double x, double y, double velX, double velY, double dirX, double dirY, int boundX, int boundY, int ID, int TIER, int timer, int health, int TimeUp)
{
	GameObject::Init(x, y, velX, velY, dirX, dirY, boundX, boundY, ID, TIER);
	Units::timer = timer;
	Units::health = health;
	Units::TimeUp = TimeUp;
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

void Units::MoveUp() {
	if (Action == MOVINGRIGHT || Action == IDLERIGHT || Action == DASHRIGHT)
		Action = MOVINGUPRIGHT;
	else
		Action = MOVINGUPLEFT;
	ActionTimer = 1;
}
void Units::MoveDown() {

	if (Action == MOVINGRIGHT || Action == IDLERIGHT || Action == DASHRIGHT)
		Action = MOVINGDOWNRIGHT;
	else
		Action = MOVINGDOWNLEFT;
	ActionTimer = 1;
}
void Units::MoveLeft() {
	Action = MOVINGLEFT;
	ActionTimer = 1;
}
void Units::MoveRight() {
	Action = MOVINGRIGHT;
	ActionTimer = 1;
}
void Units::ResetAnimation(int position)
{
	cout << "RESET" << endl;
		if (Action == MOVINGLEFT || Action == MOVINGUPLEFT || Action == MOVINGDOWNLEFT)
			Action = IDLELEFT;
		if (Action == MOVINGRIGHT || Action == MOVINGUPRIGHT || Action == MOVINGDOWNRIGHT)
			Action = IDLERIGHT;
		//dirX = 0;
		//dirY = 0;
		ActionTimer = 0;
}

void Units::Dash(double Angle) {
	dirX = sin((Angle + 90) / 180 * PI);
	dirY = cos((Angle + 90) / 180 * PI);
	if (Angle < 180)
		Action = DASHLEFT;
	else
		Action = DASHRIGHT;
	ActionTimer = 10;
}