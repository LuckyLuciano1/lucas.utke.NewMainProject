#include "Bird.h"

Bird::Bird(){}

void Bird::Destroy()
{
	Units::Destroy();
}

void Bird::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int image_size_x, int image_size_y, int copy_dir_x, int copy_dir_y, int copy_vel_x, int copy_vel_y, int copy_health)
{
	Units::Init(copy_x, copy_y, copy_vel_x, copy_vel_y, copy_dir_x, copy_dir_y, image_size_x, image_size_y, MISC, TIER1C, 0, copy_health, 0);
	SetID(MISC);
	SetAlive(true);
	SetCollidable(true);
	SetOrigCollidable(true);
	health = 5;

	maxFrame = 8;
	curFrame = 0;//horizontal
	curAnim = 0;//vertical

	frameWidth = image_size_x;
	frameHeight = image_size_y;

	TimeUp = 0;
	curAnim = IDLE;

	if (image != NULL)
		Bird::image = image;
}

void Bird::Update(double cameraX, double cameraY)
{
	Units::Update(cameraX, cameraY);
	
	timer++;
	if (timer >= 60){
		timer = 0;//restart timer every second
		curFrame++;
		if (curFrame >= maxFrame){	//comes to end of animation
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
	if (x > 0 &&//will not render if outside console window
		x < SCREENW &&
		y > 0 &&
		y < SCREENH){
		Units::Render();
		int fx = curFrame*frameWidth;
		int fy = curAnim*frameHeight;

		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y, 0);
	}
}

void Bird::StateHandler()
{
	if (curAnim == IDLE){
		maxFrame = 8;
		curFrame = 0;
	}
	else if (curAnim == PECKING){
		maxFrame = 6;
		curFrame = 0;
	}

}