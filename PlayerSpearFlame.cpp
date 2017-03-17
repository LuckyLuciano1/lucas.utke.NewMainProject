#include "PlayerSpearFlame.h"
PlayerSpearFlame::PlayerSpearFlame() {}

void PlayerSpearFlame::Destroy()
{
	GameObject::Destroy();
}

void PlayerSpearFlame::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, int ref_Multiplier)
{

	RandDirX = (rand() % 20);
	if (rand() % 2 == 1)
		RandDirX *= -1;
	RandDirY = (rand() % 100);
	RandDirY *= -1;

	RandSize = (rand() % 2 + 1)*ref_Multiplier/4;//size from roughly 10 - 30

	frameWidth = RandSize;
	frameHeight = RandSize;

	GameObject::Init(ref_x, ref_y, ref_z, 2, 2, RandDirX / 100, RandDirY / 100, frameWidth, frameHeight, PLAYERSPEARFLAME, TIER1C);

	SetCollidable(false);
	SetOrigCollidable(false);
	SetAlive(true);

	image_x = 0;
	image_y = 2000;
	
	if (image != NULL)
		PlayerSpearFlame::image = image;
}

void PlayerSpearFlame::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
	frameWidth-=2;
	frameHeight-=2;
		if ((frameWidth > 15) || (frameHeight > 15)) {
			image_y = 0;
		}
		else if ((frameWidth <= 15 && frameWidth > 10) || (frameHeight <= 15 && frameHeight > 10)) {
			image_y = 500;
		}
		else if (frameWidth <= 10 || frameHeight <= 10) {
			image_y = 1000;
		}
	if (frameWidth <= 2 || frameHeight <= 2) {
		SetAlive(false);
	}
}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void PlayerSpearFlame::Render()
{
	GameObject::Render();
	al_draw_tinted_bitmap_region(image, al_map_rgba_f(1, 1, 1, 0.3), image_x, image_y, frameWidth, frameHeight, x, y, 0);
}