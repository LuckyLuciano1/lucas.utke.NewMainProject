#include "Units.h"

Units::Units(){
	timer = 0;
	health = 0;
}//the Units class includes movecommands and basic unit AI (attacking melee, ranged, responding to commands, etc)

/*
explanation of animation and how it works for units:

curAnim (vertical) and curFrame (horizontal) both tell the position of the desired frame in the image.
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
	if (Action == LEFT)
		Action = LEFT;
	else if (Action == RIGHT)
		Action = RIGHT;
	else
		Action = LEFT;
	dirY = -1;
}
void Units::MoveDown() {
	if (Action == LEFT)
		Action = LEFT;
	else if (Action == RIGHT)
		Action = RIGHT;
	else
		Action = LEFT;
	dirY = 1;
}
void Units::MoveLeft() {
	Action = LEFT;
	dirX = -1;
}
void Units::MoveRight() {
	Action = RIGHT;
	dirX = 1;
}
void Units::ResetAnimation(int position)
{
	if (position == 1)
	{
		Action = IDLE;
		dirY = 0;
	}
	else
	{
		//Action = IDLE;
		dirX = 0;
	}
}