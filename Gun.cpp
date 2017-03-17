#include "Gun.h"

Gun::Gun() {}

void Gun::Destroy()
{
	GameObject::Destroy();
}

void Gun::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z, int image_size_x, int image_size_y, double ref_MouseAngle)
{
	GameObject::Init(ref_x, ref_y, ref_z, 6, 6, 0, 0, image_size_x, image_size_y, GUN, TIER1C);
	SetCollidable(false);
	SetOrigCollidable(false);
	SetAlive(true);

	maxFrame = 14;
	curFrame = 1;//horizontal
	frameWidth = image_size_x;
	frameHeight = image_size_y;
	curAnim = 1;//vertical
	TimeUp = 0;

	MouseAngle = ref_MouseAngle;

	if (image != NULL)
		Gun::image = image;
}

void Gun::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
	TimeUp++;
	if (TimeUp >= 2)
		SetAlive(false);
}

void Gun::Render()
{
	GameObject::Render();

	if (MouseAngle <= 67.5 && MouseAngle >= 22.5)
		curFrame = 2, curAnim = 0;
	else if (MouseAngle <= 112.5 && MouseAngle >= 67.5)
		curFrame = 1, curAnim = 0;
	else if (MouseAngle <= 157.5 && MouseAngle >= 112.5)
		curFrame = 0, curAnim = 0;
	else if (MouseAngle <= 202.5 && MouseAngle >= 157.5)
		curFrame = 0, curAnim = 1;
	else if (MouseAngle <= 247.5 && MouseAngle >= 202.5)
		curFrame = 0, curAnim = 2;
	else if (MouseAngle <= 292.5 && MouseAngle >= 247.5)
		curFrame = 1, curAnim = 2;
	else if (MouseAngle <= 337.5 && MouseAngle >= 292.5)
		curFrame = 2, curAnim = 2;
	else if ((MouseAngle <= 360 && MouseAngle >= 337.5) || (MouseAngle <= 22.5 && MouseAngle >= 0))
		curFrame = 2, curAnim = 1;


	int fx = curFrame * frameWidth;
	int fy = curAnim * frameHeight;
	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x + frameWidth / 2 - ((2 - curFrame)*frameWidth / 2), y + frameHeight / 2 - ((2 - curAnim)*frameHeight / 2), 0);

	//cout << MouseAngle << endl;
	//al_draw_tinted_scaled_rotated_bitmap_region(image, fx, fy, frameWidth, frameHeight, al_map_rgba(1, 1, 1, 1), x - ((2 - curFrame)*frameWidth / 2), y - ((2 - curAnim)*frameHeight / 2), x - ((2 - curFrame)*frameWidth / 2), y - ((2 - curAnim)*frameHeight / 2), 1, 1, MouseAngle, 0);
}