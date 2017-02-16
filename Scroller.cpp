#include "Scroller.h"
#include <iostream>
using namespace std;

Scroller::Scroller(ALLEGRO_BITMAP *image)
{
	GameObject::Init(0, 0, .25, .25, 1, 0, 0, 0, DETAIL, TIER2A);

	SetID(DETAIL);
	SetCollidable(false);
	SetOrigCollidable(false);

	Scroller::image = image;
	frameWidth = al_get_bitmap_width(image);
}

void Scroller::Destroy()
{
	GameObject::Destroy();
}

void Scroller::Update(double cameraX, double cameraY)
{
	if (x >= WORLDW)
		x = 0;

	GameObject::Update(cameraX, cameraY);
}

void Scroller::Render()
{
	al_draw_tinted_bitmap(image, al_map_rgba_f(-255, -255, -255, 0.25), x, y, 0);

	if (x > 0) {
		al_draw_tinted_bitmap(image, al_map_rgba_f(-255, -255, -255, 0.25), x - WORLDW, y, 0);
	}
}