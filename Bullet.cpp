#include "Bullet.h"

Bullet::Bullet() {}

void Bullet::Destroy()
{
	Units::Destroy();
}

void Bullet::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int image_size_x, int image_size_y, double copy_MouseAngle, double copy_dirX, double copy_dirY)
{
	Units::Init(copy_x, copy_y, 20, 20, copy_dirX, copy_dirY, image_size_x, image_size_y, PROJECTILE, TIER1C, 0, 5, 200);

	SetID(PROJECTILE);
	SetAlive(true);
	SetCollidable(true);
	SetOrigCollidable(true);

	maxFrame = 14;
	curFrame = 1;//horizontal
	MouseAngle = copy_MouseAngle;

	srand(time(NULL));
	if (image != NULL)
		Bullet::image = image;
}

void Bullet::Update(double cameraX, double cameraY)
{
	Units::Update(cameraX, cameraY);

	MouseAngle += .25;
	if (MouseAngle > 360)
		MouseAngle = 0;

	TimeUp--;
	if (TimeUp == 0)
		SetAlive(false);
}

void Bullet::Render()
{
		Units::Render();
		int fx = curFrame * frameWidth;
		int fy = curAnim * frameHeight;

		al_draw_rotated_bitmap(image, frameWidth / 2, frameHeight / 2, x + frameWidth / 2, y + frameHeight / 2, MouseAngle, 0);
		//al_draw_tinted_bitmap(image, al_map_rgba_f(1, 1, 1, 0.5), x + frameWidth / 2, y + frameHeight / 2, 0);
}