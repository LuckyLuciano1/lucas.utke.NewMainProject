#include "ShadowedBlock.h"

ShadowedBlock::ShadowedBlock() {}

void ShadowedBlock::Destroy()
{
	GameObject::Destroy();
}

void ShadowedBlock::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, double image_size_x, double image_size_y)
{
	GameObject::Init(ref_x, ref_y, ref_z, 6, 6, 0, 0, image_size_x, image_size_y, SHADOWEDBLOCK, TIER0C);

	SetCollidable(false);
	SetOrigCollidable(false);

	SetAlive(true);

	frameWidth = image_size_x;
	frameHeight = image_size_y;
	image_x = 0;
	image_y = 18;
	if (image != NULL)
		ShadowedBlock::image = image;
}

void ShadowedBlock::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void ShadowedBlock::Render()
{
	GameObject::Render();
	al_draw_tinted_bitmap_region(image, al_map_rgba_f(127, 127, 127, .6), image_x*DIMW, image_y*DIMH, frameWidth, frameHeight, x, y, 0);
}