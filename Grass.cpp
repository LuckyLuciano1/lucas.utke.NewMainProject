#include "Grass.h"

Grass::Grass() {}

void Grass::Destroy()
{
	GameObject::Destroy();
}

void Grass::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, double image_size_x, double image_size_y)
{
	GameObject::Init(ref_x, ref_y, ref_z, 6, 6, 0, 0, image_size_x, image_size_y, GRASS, TIER1C);

	SetCollidable(false);
	SetOrigCollidable(false);

	SetAlive(true);

	frameWidth = image_size_x;
	frameHeight = image_size_y;

	curAnim = DIMH*rand() % 3;
	curFrame = rand() % 2;
	maxFrame = 3;

	timer = rand() % 30;

	if (image != NULL)
		Grass::image = image;
}

void Grass::Update(double CameraX, double CameraY, vector<GameObject*> &objects)
{
	GameObject::Update(CameraX, CameraY, objects);
	timer++;
	if (timer >= 30) {
		timer = 0;
		curFrame++;
		if (curFrame >= maxFrame)
			curFrame = 0;
	}
}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void Grass::Render()
{
	GameObject::Render();
	int fx = curFrame*frameWidth;
	int fy = curAnim*frameHeight;
	al_draw_bitmap_region(image, image_x + (curFrame*frameWidth), image_y*frameHeight, frameWidth, frameHeight, x, y, 0);
}