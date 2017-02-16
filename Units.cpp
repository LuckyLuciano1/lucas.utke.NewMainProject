#include "Units.h"

Units::Units(){
	timer = 0;
	health = 0;
}//the Units class includes movecommands and basic unit AI (attacking melee, ranged, responding to commands, etc)

void Units::Init(double x, double y, double velX, double velY, double dirX, double dirY, int boundX, int boundY, int ID, int TIER, int timer, int health, int TimeUp)
{
	GameObject::Init(x, y, velX, velY, dirX, dirY, boundX, boundY, ID, TIER);
	Units::timer = timer;
	Units::health = health;
	Units::TimeUp = TimeUp;
}
void Units::Update(double CameraX, double CameraY)
{
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
	curAnim = MOVING;
	dirY = -1;
}
void Units::MoveDown() {
	curAnim = MOVING;
	dirY = 1;
}
void Units::MoveLeft() {
	curAnim = MOVING;
	dirX = -1;
}
void Units::MoveRight() {
	curAnim = MOVING;
	dirX = 1;
}
void Units::ResetAnimation(int position)
{
	if (position == 1)
	{
		curAnim = IDLE;
		dirY = 0;
	}
	else
	{
		curAnim = IDLE;
		dirX = 0;
	}
}