#include "Bullet.h"

Bullet::Bullet() {}

void Bullet::Destroy()
{
	GameObject::Destroy();
}

void Bullet::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int image_size_x, int image_size_y, double copy_MouseAngle, double copy_dirX, double copy_dirY)
{
	GameObject::Init(copy_x, copy_y, 20, 20, copy_dirX, copy_dirY, image_size_x, image_size_y, BULLET, TIER1C);

	SetAlive(true);
	SetCollidable(true);
	SetOrigCollidable(true);

	Angle = copy_MouseAngle;

	srand(time(NULL));
	if (image != NULL)
		Bullet::image = image;
}

void Bullet::Update(double cameraX, double cameraY)
{
	GameObject::Update(cameraX, cameraY);

	Angle += .25;
	if (Angle > 360)
		Angle = 0;

	TimeUp--;
	if (TimeUp == 0)
		SetAlive(false);
}

void Bullet::Render()
{
	GameObject::Render();

	al_draw_rotated_bitmap(image, frameWidth / 2, frameHeight / 2, x + frameWidth / 2, y + frameHeight / 2, Angle, 0);
}