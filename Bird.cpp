#include "Bird.h"

Bird::Bird() {}

void Bird::Destroy()
{
	Units::Destroy();
}

void Bird::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, int image_size_x, int image_size_y, int ref_dir_x, int ref_dir_y, int ref_vel_x, int ref_vel_y, int ref_health)
{
	Units::Init(ref_x, ref_y, ref_vel_x, ref_vel_y, ref_dir_x, ref_dir_y, image_size_x, image_size_y, BIRD, TIER1C, 0, ref_health);
	SetAlive(true);
	SetCollidable(true);
	SetOrigCollidable(true);
	health = 5;

	maxFrame = 8;
	curFrame = 0;//horizontal
	curAnim = 0;//vertical

	frameWidth = image_size_x;
	frameHeight = image_size_y;

	curAnim = IDLE;

	if (image != NULL)
		Bird::image = image;
}

void Bird::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	Units::Update(cameraX, cameraY, objects);

	timer++;
	if (timer >= 60) {
		timer = 0;//restart timer every second
		curFrame++;
		if (curFrame >= maxFrame) {	//comes to end of animation
			if (rand() % 3 == 1) {
				curAnim = PECKING;
			}
			else {
				curAnim = IDLE;
			}
			StateHandler();
		}
	}
}

void Bird::Render()
{
	Units::Render();
	int fx = curFrame*frameWidth;
	int fy = curAnim*frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y, 0);
}

void Bird::StateHandler()
{
	if (curAnim == IDLE) {
		maxFrame = 8;
		curFrame = 0;
	}
	else if (curAnim == PECKING) {
		maxFrame = 6;
		curFrame = 0;
	}

}