#include "Background.h"

Background::Background(ALLEGRO_BITMAP *image)
{
	GameObject::Init(0, 0, 0, 0, 0, 0, 0, 0, 0, BACKGROUND, TIER0A);

	SetCollidable(false);
	SetOrigCollidable(false);

	Background::image = image;
	frameWidth = al_get_bitmap_width(image);
}

void Background::Destroy()
{
	GameObject::Destroy();
}

void Background::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
}

void Background::Render()
{
	al_draw_bitmap(image, x, y, 0);
}