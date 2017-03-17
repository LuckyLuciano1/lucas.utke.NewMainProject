#include "Cultist.h"

Cultist::Cultist() {}

void Cultist::Destroy()
{
	Units::Destroy();
}

void Cultist::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, double ref_z)
{
	frameWidth = 39;
	frameHeight = 96;
	Units::Init(ref_x, ref_y, ref_z, PLAYERVELX - 1, PLAYERVELY - 1, 0, 0, frameWidth, frameHeight, CULTIST, TIER1C, 0, 5);
	SetAlive(true);
	SetCollidable(false);
	SetOrigCollidable(false);

	health = 5;
	timer = 0;

	maxFrame = 4;
	curFrame = 1;

	TargetFound = false;

	Animation = IDLELEFT;
	AnimationHandler();

	if (image != NULL)
		Cultist::image = image;
}

void Cultist::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	Units::Update(cameraX, cameraY, objects);

	if (TargetFound == false) {//searches for target once, then never again.
		for (iter = objects.begin(); iter != objects.end(); ++iter)
		{
			if ((*iter)->GetID() == PLAYER)
				Target = (*iter);
		}
		TargetFound = true;
	}
	else {
		//if(abs(Target->GetX() - x) < 500 && abs(Target->GetY() - y) < 500)
		Orbit(Target);
	}
}

void Cultist::Render()
{
	Units::Render();
	int fx = curFrame*frameWidth;
	int fy = curAnim*frameHeight;
	if(Animation == MOVINGLEFT)
		al_draw_tinted_bitmap_region(image, al_map_rgba_f(225, 225, 225, 0.5), 160, 0, 36, 18, x + 15, BaseY - 12, 0);//shadow underneath character
	else
		al_draw_tinted_bitmap_region(image, al_map_rgba_f(225, 225, 225, 0.5), 160, 0, 36, 18, x, BaseY - 12, 0);//shadow underneath character

	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y, 0);
}

//sets up the various variables that come alongside the Animation states. called whenever Animation is changed
void Cultist::AnimationHandler()
{
	//dimensions and other variables will default to:
	frameWidth = 39;
	frameHeight = 96;
	boundX = 39;
	boundY = 96;

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
		frameWidth = 51;
		boundX = 51;
	}
	else if (Animation == MOVINGRIGHT) {
		curAnim = 3;
		maxFrame = 4;
		frameWidth = 51;
		boundX = 51;
	}
	else if (Animation == DASHLEFT) {
		cout << "DASHLEFT is not finished. get off your ass and finish it, future self." << endl;
	}
	else if (Animation == DASHRIGHT) {
		cout << "DASHRIGHT is not finished. get off your ass and finish it, future self." << endl;
	}
}

void Cultist::Orbit(GameObject *Target) {
	Units::Orbit(Target);
	AnimationHandler();
}