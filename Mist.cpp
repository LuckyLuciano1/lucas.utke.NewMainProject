#include "Mist.h"

Mist::Mist() {}

void Mist::Destroy()
{
	GameObject::Destroy();
}

void Mist::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int copy_MistID)
{

	RandDirX = (rand() % 20);
	if (rand() % 2 == 1)
		RandDirX *= -1;
	RandDirY = (rand() % 100);
	//if (rand() % 2 == 1)
		RandDirY *= -1;

	RandSize = rand() % 30 + 10;//size from 10 - 30
	frameWidth = RandSize;
	frameHeight = RandSize;

	GameObject::Init(copy_x, copy_y, 2, 2, RandDirX / 100, RandDirY / 100, frameWidth, frameHeight, MIST, TIER1C);

	SetCollidable(false);
	SetOrigCollidable(false);
	SetAlive(true);


	MistID = copy_MistID;
	image_x = 0;
	if (MistID == FIRE)
		image_y = 0;
	else if (MistID == SMOKE)
		image_y = 1500;
	else
		image_y = 0;

	if (image != NULL)
		Mist::image = image;
}

void Mist::Update(double cameraX, double cameraY)
{
	GameObject::Update(cameraX, cameraY);
	frameWidth-=.5;
	frameHeight-=.5;
	if (MistID == FIRE) {
		if ((frameWidth > 15) || (frameHeight > 15)) {
			image_y = 0;
		}
		else if ((frameWidth <= 15 && frameWidth > 10) || (frameHeight <= 15 && frameHeight > 10)) {
			image_y = 500;
		}
		else if (frameWidth <= 10 || frameHeight <= 10) {
			image_y = 1000;
		}
	}
	if (frameWidth <= 2 || frameHeight <= 2) {
		SetAlive(false);
	}
}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void Mist::Render()
{
	GameObject::Render();
	al_draw_tinted_bitmap_region(image, al_map_rgba_f(1, 1, 1, 0.3), image_x, image_y, frameWidth, frameHeight, x - frameWidth/2, y-frameHeight/2, 0);
}