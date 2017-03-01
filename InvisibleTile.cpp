#include "InvisibleTile.h"

InvisibleTile::InvisibleTile() {}
//this is basically just the terrain tile. However, It does not render, and may contain other later properties.
void InvisibleTile::Destroy()
{
	GameObject::Destroy();
}

void InvisibleTile::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_image_x, double ref_image_y, double image_size_x, double image_size_y, int TIER)
{
	GameObject::Init(ref_x, ref_y, 6, 6, 0, 0, image_size_x, image_size_y, INVISIBLETILE, TIER);

	//collision will always be true
	SetCollidable(true);
	SetOrigCollidable(false);

	SetAlive(true);

	frameWidth = image_size_x;
	frameHeight = image_size_y;
	image_x = ref_image_x;
	image_y = ref_image_y;
	if (image != NULL)
		InvisibleTile::image = image;
}

void InvisibleTile::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
}

void InvisibleTile::Render()
{
	//does not render. (it is invisible).
}