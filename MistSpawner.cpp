#include "MistSpawner.h"

MistSpawner::MistSpawner() {}

void MistSpawner::Destroy()
{
	GameObject::Destroy();
}

void MistSpawner::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, double copy_image_x, double copy_image_y, double image_size_x, double image_size_y, bool collision)
{
	GameObject::Init(copy_x, copy_y, 6, 6, 0, 0, image_size_x, image_size_y, MISC, TIER1C);

	SetCollidable(false);
	SetOrigCollidable(false);

	SetID(MISC);

	SetAlive(true);

	frameWidth = image_size_x;
	frameHeight = image_size_y;
	image_x = copy_image_x;
	image_y = copy_image_y;
	frameCounter = 0;
	if (image != NULL)
		MistSpawner::image = image;
}

void MistSpawner::Update(double cameraX, double cameraY)
{
	GameObject::Update(cameraX, cameraY);
	frameCounter++;
	if (frameCounter >= 60) {
		frameCounter = 0;
		//spawn mist here?
	}
}

//does not render, only meant to spawn mist
void MistSpawner::Render()
{}