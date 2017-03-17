#include "Bullet.h"

Bullet::Bullet() {}

void Bullet::Destroy()
{
	GameObject::Destroy();
}

void Bullet::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, int image_size_x, int image_size_y, double ref_MouseAngle, double ref_dirX, double ref_dirY)
{
	GameObject::Init(ref_x, ref_y, ref_z, 20, 20, ref_dirX, ref_dirY, image_size_x, image_size_y, BULLET, TIER1C);

	SetAlive(true);
	SetCollidable(true);
	SetOrigCollidable(true);

	Angle = ref_MouseAngle;

	srand(time(NULL));
	if (image != NULL)
		Bullet::image = image;
}

void Bullet::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);

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