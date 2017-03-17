#pragma once

#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "Globals.h"
#include <vector>
using namespace std;

class GameObject
{
private:
	int ID;
	bool alive;
	bool origcollidable;//the original ability for an object to collide
	bool collision;//the collision of two objects
	bool collidable;//the ability for an object to collide
	bool renderable;//the ability for an object to render

protected:
	double x;//position of object within game
	double y;

	double z;//pseudo-height variable. does not require Vel or Dir, as it merely adds to y to produce displacement.

	double velX;//speed of object within game
	double velY;

	double dirX;//direction of object within game
	double dirY;

	int boundX;//size of object within game
	int boundY;


	double frameWidth;//size of image being displayed
	double frameHeight;

	int maxFrame;//end of animation, requires loop
	int curFrame;//current frame that animation is on
	double curAnim;//current animation that is being played (primarily for Units, otherwise permeneantly 0)

	ALLEGRO_BITMAP *image;

public:

	GameObject();
	void virtual Destroy();

	int TIER;//these are public in order to properly sort objects with minimal processing
	double BaseY;

	void Init(double x, double y, double z, double velX, double velY, double dirX, double dirY, int boundX, int boundY, int ID, int TIER);
	void virtual Update(double cameraX, double cameraY, vector<GameObject*> &objects);
	void virtual Render();

	double GetX() { return x; }
	double GetY() { return y; }
	double GetZ() { return z; }

	void SetX(double x) { GameObject::x = x; }
	void SetY(double y) { GameObject::y = y; }
	void SetZ(double z) { GameObject::z = z; }

	double GetDirX() { return dirX; }
	double GetDirY() { return dirY; }

	void SetDirX(double dirX) { GameObject::dirX = dirX; }
	void SetDirY(double dirY) { GameObject::dirY = dirY; }

	int GetBoundX() { return boundX; }
	int GetBoundY() { return boundY; }

	int GetVelX() { return velX; }
	int GetVelY() { return velY; }

	int GetID() { return ID; }
	void SetID(int ID) { GameObject::ID = ID; }

	double GetBaseY() { return BaseY; }
	void SetBaseY(double BaseY) { GameObject::BaseY = BaseY; }

	bool GetCollision() { return collision; };
	void SetCollision(bool collision) { GameObject::collision = collision; };

	bool GetRender() { return renderable; };
	void SetRender(bool renderable) { GameObject::renderable = renderable; };

	bool GetAlive() { return alive; }
	void SetAlive(bool alive) { GameObject::alive = alive; }

	bool GetCollidable() { return collidable; }
	void SetCollidable(bool collidable) { GameObject::collidable = collidable; }

	bool GetOrigCollidable() { return origcollidable; }
	void SetOrigCollidable(bool origcollidable) { GameObject::origcollidable = origcollidable; }

	bool CheckCollisions(GameObject *otherObject);
	void virtual Collided(GameObject *otherObject);
	bool Collidable();
};