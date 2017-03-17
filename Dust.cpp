#include "Dust.h"
Dust::Dust() {}

void Dust::Destroy()
{
	GameObject::Destroy();
}

void Dust::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, int ref_SpawnSize_x, int ref_SpawnSize_y)
{
	RandSize = rand() % 20;
	frameWidth = RandSize;
	frameHeight = RandSize;
	x = rand() % ref_SpawnSize_x + ref_x;
	y = rand() % ref_SpawnSize_y + ref_y;
	GameObject::Init(x, y, ref_z, 2, 2, .5, .5, frameWidth, frameHeight, DUST, TIER1C);

	SetCollidable(false);
	SetOrigCollidable(false);
	SetAlive(true);


	image_x = 0;
	image_y = 3000;

	if (image != NULL)
		Dust::image = image;
}

void Dust::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
	frameWidth-=.5;
	frameHeight-=.5;
	x--;
	y--;

	if (frameWidth <= 2 || frameHeight <= 2) {
		SetAlive(false);
	}
}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void Dust::Render()
{
	GameObject::Render();
	al_draw_tinted_bitmap_region(image, al_map_rgba_f(1, 1, 1, 0.5), image_x, image_y, frameWidth, frameHeight, x, y, 0);
}