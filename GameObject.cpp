#include "GameObject.h"
#include <math.h>
#include <iostream>
using namespace std;

GameObject::GameObject()
{
	x = 0;
	y = 0;
	z = 0;

	velX = 0;
	velY = 0;

	dirX = 0;
	dirY = 0;

	boundX = 0;
	boundY = 0;

	BaseY = 0;

	frameWidth = 0;
	frameHeight = 0;

	maxFrame = 0;
	curFrame = 0;
	curAnim = 0;

	image = NULL;

	alive = true;
	collidable = true;
	origcollidable = false;
}

void GameObject::Destroy() {}

void GameObject::Init(double x, double y, double z, double velX, double velY, double dirX, double dirY, int boundX, int boundY, int ID, int TIER)
{
	GameObject::x = x;
	GameObject::y = y;// +z;//position
	GameObject::z = z;

	GameObject::velX = velX;
	GameObject::velY = velY;//speed

	GameObject::dirX = dirX;
	GameObject::dirY = dirY;//direction

	GameObject::boundX = boundX;
	GameObject::boundY = boundY;//size

	GameObject::BaseY = y + boundY;

	GameObject::ID = ID;
	GameObject::TIER = TIER;

}

void GameObject::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	x += velX * dirX;
	y += velY * dirY;

	x += cameraX;
	y += cameraY;

	BaseY = y + boundY;

	if (origcollidable == true) {
		if (x > -frameWidth * 2 &&//will not collide if outside console window
			x < SCREENW + frameWidth * 2 &&
			y > -frameHeight * 2 &&
			y < SCREENH + frameHeight * 2)
			collidable = true;
		else
			collidable = false;
	}

	if (x > -frameWidth * 2 &&//will not render if outside console window
		x < SCREENW + frameWidth * 2 &&
		y > -frameHeight * 2 &&
		y < SCREENH + frameHeight * 2)
		renderable = true;
	else
		renderable = false;

}

void GameObject::Render()
{}

bool GameObject::CheckCollisions(GameObject *otherObject)
{
	collision = false;
	double oX = otherObject->GetX();
	double oY = otherObject->GetY();

	int obX = otherObject->GetBoundX();
	int obY = otherObject->GetBoundY();

	double onewX = (otherObject->GetVelX()*otherObject->GetDirX());
	double onewY = (otherObject->GetVelY()*otherObject->GetDirY());

	double newX = velX *dirX;
	double newY = velY *dirY;

	if (x + boundX + newX > oX + onewX &&
		x + newX < oX + obX + onewX &&
		y + boundY + newY > oY + onewY &&
		y + newY < oY + obY + onewY)
		return true;
	else
		return false;
}

void GameObject::Collided(GameObject *otherObject)
{
	collision = true;
	if (ID == CULTIST && (otherObject)->GetID() == BULLET) {
		SetAlive(false);
		(otherObject)->SetAlive(false);
	}
	if (ID == PLAYER && (otherObject)->GetID() == TERRAIN) {
		double oX = otherObject->GetX();
		double oY = otherObject->GetY();

		int obX = otherObject->GetBoundX();
		int obY = otherObject->GetBoundY();

		double onewX = (otherObject->GetVelX()*otherObject->GetDirX());
		double onewY = (otherObject->GetVelY()*otherObject->GetDirY());

		double newX = velX *dirX;
		double newY = 0;//if collision still works without Y, 

		if (x + boundX + newX > oX + onewX &&
			x + newX < oX + obX + onewX &&
			y + boundY + newY > oY + onewY &&
			y + newY < oY + obY + onewY) {
			//then Y doesnt impact collision
			dirX = 0;
		}

		newX = 0;//same for X
		newY = velY *dirY;

		if (x + boundX + newX > oX + onewX &&
			x + newX < oX + obX + onewX &&
			y + boundY + newY > oY + onewY &&
			y + newY < oY + obY + onewY) {
			//then X doesnt impact collision
			dirY = 0;
		}
	}
}

bool GameObject::Collidable()
{
	return alive && collidable;
}