#include "MistSpawner.h"
#include "Mist.h"
MistSpawner::MistSpawner() {}

void MistSpawner::Destroy()
{
	GameObject::Destroy();
}

void MistSpawner::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, int ref_MistID)
{
	GameObject::Init(ref_x, ref_y, PLAYERVELX - 1, PLAYERVELY - 1, 0, 0, 30, 30, MISTSPAWNER, TIER1C);
	SetCollidable(false);
	SetOrigCollidable(false);

	SetAlive(true);

	frameWidth = 30;
	frameHeight = 30;
	frameCounter = 0;
	MistID = ref_MistID;
	TimeUp = 60;
	TargetFound = false;
	if (image != NULL)
		MistSpawner::image = image;
}

void MistSpawner::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
	frameCounter++;
	if (frameCounter >= 60) {
		frameCounter = 0;
		//this randomizes the velocity slightly, to make the movement of the sprite more pondering and organic
		int randnum = rand() % 1 + 1;
		if (rand() % 2 == 1) {
			randnum *= -1;
		}
		velX += randnum, velY += randnum;
		if (velX >= 6) {
			velX--;
			velY--;
		}
		else if (velX <= 2) {
			velX++;
			velY++;
		}
	}

	//TimeUp--;
	if (TimeUp <= 0)
		SetAlive(false);

	Mist *mist = new Mist();//creates mist effect. basis of entire object.
	mist->Init(image, x, y, MistID);
	objects.push_back(mist);

	if (TargetFound == false) {//searches for target once, then never again.
		for (iter = objects.begin(); iter != objects.end(); ++iter)
		{
			if ((*iter)->GetID() == PLAYER)
				Target = (*iter);
		}
		TargetFound = true;
	}
	else
		Orbit(Target);
}

//does not render, only meant to spawn mist
void MistSpawner::Render()
{
	//small box/rune symbol to indicate center of effect
	al_draw_tinted_bitmap_region(image, al_map_rgba_f(1, 1, 1, 0.9), 0, 2500, frameWidth, frameHeight, x, y, 0);
}

void MistSpawner::Orbit(GameObject *Target) {
	double oX = Target->GetX();
	double oY = Target->GetY();
	int obX = Target->GetBoundX();
	int obY = Target->GetBoundY();
	int hypotenuse = sqrt(((y - (oY + (obY / 2)))*(y - (oY + (obY / 2)))) + ((x - (oX + (obX / 2)))*(x - (oX + (obX / 2)))));//finds the length of the distance between object and target

	if (hypotenuse > 100) {//contracting spiral
		dirX = -sin(atan2(y - (oY + (obY / 2)), x - (oX + (obX / 2))) + 120);// the plus 90 adjusts the angle. without it, the unit will orbit the target.
		dirY = cos(atan2(y - (oY + (obY / 2)), x - (oX + (obX / 2))) + 120);
	}
	else if(hypotenuse < 95) {//expanding spiral
		dirX = sin(atan2(y - (oY + (obY / 2)), x - (oX + (obX / 2))) + 120);// the plus 90 adjusts the angle. without it, the unit will orbit the target.
		dirY = -cos(atan2(y - (oY + (obY / 2)), x - (oX + (obX / 2))) + 120);
	}
	else {//maintaining orbit
		dirX = -sin(atan2(y - (oY + (obY / 2)), x - (oX + (obX / 2))) + 0);// the plus 90 adjusts the angle. without it, the unit will orbit the target.
		dirY = cos(atan2(y - (oY + (obY / 2)), x - (oX + (obX / 2))) + 0);
	}
}