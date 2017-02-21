#include "ShadowedBlock.h"

ShadowedBlock::ShadowedBlock() {}

void ShadowedBlock::Destroy()
{
	GameObject::Destroy();
}

void ShadowedBlock::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, double image_size_x, double image_size_y)
{
	GameObject::Init(copy_x, copy_y, 6, 6, 0, 0, image_size_x, image_size_y, DETAIL, TIER0C);

	SetCollidable(false);
	SetOrigCollidable(false);

	SetID(DETAIL);

	SetAlive(true);

	frameWidth = image_size_x;
	frameHeight = image_size_y;
	image_x = 0;
	image_y = 18;
	if (image != NULL)
		ShadowedBlock::image = image;
}

void ShadowedBlock::Update(double cameraX, double cameraY)
{
	GameObject::Update(cameraX, cameraY);
}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void ShadowedBlock::Render()
{
		GameObject::Render();
		al_draw_tinted_bitmap_region(image, al_map_rgba_f(127, 127, 127, .6), image_x*DIMW, image_y*DIMH, frameWidth, frameHeight, x, y, 0);
}