#include "InvisibleTile.h"

InvisibleTile::InvisibleTile() {}
//this is basically just the terrain tile. However, It does not render, and may contain other later properties.
void InvisibleTile::Destroy()
{
	GameObject::Destroy();
}

void InvisibleTile::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, double copy_image_x, double copy_image_y, double image_size_x, double image_size_y, int TIER)
{
	GameObject::Init(copy_x, copy_y, 6, 6, 0, 0, image_size_x, image_size_y, OBSTACLE, TIER);

	//collision will always be true
	SetCollidable(true);
	SetOrigCollidable(false);
	SetID(OBSTACLE);

	SetAlive(true);

	frameWidth = image_size_x;
	frameHeight = image_size_y;
	image_x = copy_image_x;
	image_y = copy_image_y;
	if (image != NULL)
		InvisibleTile::image = image;
}

void InvisibleTile::Update(double cameraX, double cameraY)
{
	GameObject::Update(cameraX, cameraY);
}

void InvisibleTile::Render()
{
	//does not render. (it is invisible).
}