#include "MistSpawner.h"
#include "Mist.h"

MistSpawner::MistSpawner() {}

void MistSpawner::Destroy()
{
	GameObject::Destroy();
}

void MistSpawner::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int copy_MistID)
{
	GameObject::Init(copy_x, copy_y, 5, 5, 0, 0, 1, 1, MISTSPAWNER, TIER1C);

	SetCollidable(false);
	SetOrigCollidable(false);

	SetAlive(true);

	frameWidth = 1;//as small as possible
	frameHeight = 1;
	frameCounter = 0;
	MistID = copy_MistID;
	TimeUp = 200;

	if (image != NULL)
		MistSpawner::image = image;
}

void MistSpawner::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
	frameCounter++;
	if (frameCounter >= 60) {
		frameCounter = 0;
	}
	//TimeUp--;
	if (TimeUp <= 0)
		SetAlive(false);

	Mist *mist = new Mist();
	mist->Init(image, x, y, MistID);
	objects.push_back(mist);

	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if ((*iter)->GetID() == PLAYER)
			Pursue((*iter));
	}
}

//does not render, only meant to spawn mist
void MistSpawner::Render()
{}

void MistSpawner::Pursue(GameObject *otherObject) {
	double oX = otherObject->GetX();
	double oY = otherObject->GetY();
	int obX = otherObject->GetBoundX();
	int obY = otherObject->GetBoundY();

	dirX = -sin(atan2(y - (oY + (obY / 2)), x - (oX + (obX / 2))) + 160);// the plus 90 adjusts the angle. without it, the unit will orbit the target.
	dirY = cos(atan2(y - (oY + (obY / 2)), x - (oX + (obX / 2))) + 160);
}