#include "Cultist.h"

Cultist::Cultist() {}

void Cultist::Destroy()
{
	Units::Destroy();
}

void Cultist::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y)
{
	frameWidth = 39;
	frameHeight = 96;
	Units::Init(copy_x, copy_y, 5, 5, 0, 0, frameWidth, frameHeight, CULTIST, TIER1C, 0, 5);
	SetAlive(true);
	SetCollidable(true);
	SetOrigCollidable(true);

	health = 5;
	timer = 0;

	maxFrame = 4;
	curFrame = 1;

	Animation = IDLELEFT;
	AnimationHandler();

	if (image != NULL)
		Cultist::image = image;
}

void Cultist::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	Units::Update(cameraX, cameraY, objects);
	AnimationHandler();//temp
}

void Cultist::Render()
{
	Units::Render();
	int fx = curFrame*frameWidth;
	int fy = curAnim*frameHeight;

	al_draw_tinted_bitmap_region(image, al_map_rgba_f(225, 225, 225, 0.5), frameWidth * 4, frameHeight * 0, 36, 18, x, BaseY - 12, 0);//shadow underneath character
	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y, 0);
}

//sets up the various variables that come alongside the Animation states. called whenever Animation is changed
void Cultist::AnimationHandler()
{
	//dimensions and other variables will default to:
	//frameWidth = 39;
	//frameHeight = 96;
	//boundX = 39;
	//boundY = 96;

	if (Animation == IDLELEFT) {
		curAnim = 0;
		maxFrame = 4;
	}
	else if (Animation == IDLERIGHT) {
		curAnim = 1;
		maxFrame = 4;
	}
	else if (Animation == MOVINGLEFT) {
		curAnim = 2;
		maxFrame = 4;
	}
	else if (Animation == MOVINGRIGHT) {
		curAnim = 3;
		maxFrame = 4;
	}
	else if (Animation == DASHLEFT) {
		cout << "DASHLEFT is not finished. get off your ass and finish it, future self." << endl;
	}
	else if (Animation == DASHRIGHT) {
		cout << "DASHRIGHT is not finished. get off your ass and finish it, future self." << endl;
	}
}

void Cultist::Pursue(GameObject *otherObject) {
	cout << "cultist pursue" << endl;
	Units::Pursue(otherObject);
	AnimationHandler();
}