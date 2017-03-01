#include "CloudBase.h"

CloudBase::CloudBase() {}

void CloudBase::Destroy()
{
	GameObject::Destroy();
}

void CloudBase::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_image_x, double ref_image_y, bool stable)
{
	GameObject::Init(ref_x, ref_y, 6, 6, 0, 0, DIMW, DIMH, CLOUDBASE, TIER0A);
	SetCollidable(false);
	SetOrigCollidable(false);

	SetAlive(true);

	frameWidth = DIMW;
	frameHeight = DIMH;
	image_x = ref_image_x;
	image_y = ref_image_y;

	if (stable == true)
		CloudState = PERMASTABLE;//perma stable is for the clouds underneath, which need to form a solid base.
	else {
		if (rand() % 2 == 1)
			CloudState = GROWING;
		else
			CloudState = SHRINKING;

		frameHeight = rand() % ((DIMH * 2) - (DIMH / 2)) + (DIMH / 2);
		frameWidth = frameHeight;
	}

	if (image != NULL)
		CloudBase::image = image;
}

void CloudBase::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);

	if (CloudState == SHRINKING && (frameHeight <= DIMH / 2 || frameWidth <= DIMW / 2))//if cloud gets to small, make it regain size
		CloudState = GROWING;

	if (CloudState == GROWING && (frameHeight >= DIMH * 2 || frameWidth >= DIMW * 2))//if gets too large, make it shrink
		CloudState = SHRINKING;

	if (CloudState == SHRINKING && rand() % 600 == 1)//chance to start growing while shrinking
		CloudState = GROWING;
	if (CloudState == GROWING && rand() % 600 == 1)//chance to start shrinking while growing
		CloudState = SHRINKING;




	if (CloudState == SHRINKING) {
		frameWidth -= .25;
		frameHeight -= .25;
	}
	if (CloudState == GROWING) {
		frameWidth += .25;
		frameHeight += .25;
	}

}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void CloudBase::Render()
{
	GameObject::Render();
	al_draw_tinted_bitmap_region(image, al_map_rgba_f(125, 125, 125, 0.5), image_x, image_y, frameWidth, frameHeight, x + (DIMW - frameWidth) / 2, y + (DIMH - frameHeight) / 2, 0);
}