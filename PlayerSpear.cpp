#include "PlayerSpear.h"
#include "Mist.h"

#define PI 3.14159265
#define DEGREES(x) int((x)/360.0*0xFFFFFF)
#define RADIANS(x) int((x)/2/M_PI*0xFFFFFF)

PlayerSpear::PlayerSpear() {}

void PlayerSpear::Destroy()
{
	GameObject::Destroy();
}

void PlayerSpear::Init(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *ref_ColorImage, double ref_x, double ref_y, double ref_SpearAngle)
{
	GameObject::Init(ref_x, ref_y, PLAYERVELX, PLAYERVELY, 0, 0, 1, 1, PLAYERSPEAR, TIER1C);
	SetCollidable(false);
	SetOrigCollidable(false);

	SetAlive(true);

	frameWidth = 1;//starts in center of image and 1x1 box. every update, will grow significantly while recentering. This will give the appearance of the spear being summoned.
	frameHeight = 1;
	image_x = 226;
	image_y = 34;
	//SpearAngle = 0;
	SpearAngle = (ref_SpearAngle);//image of spear is angled at 315 (or -45) degrees from start
	SpearState = SPINNING;
	SpearTipX = 0;
	SpearTipY = 0;

	ColorImage = ref_ColorImage;
	if (image != NULL)
		PlayerSpear::image = image;
}

void PlayerSpear::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
	if (image_y != 0 && frameWidth < 69 && frameHeight < 69) {//makes Spear expand upon creation
		frameWidth+=2;
		frameHeight+=2;
		image_x--;
		image_y--;
	}
	/*if (SpearState == SPINNING) {
		SpearAngle+=10;
	}
	if (SpearAngle >= 360)
		SpearAngle = 0;*/

	//determines position of spear tip
	SpearTipX = x + (cos((SpearAngle  + 315) / 180 * PI)*(sqrt((frameWidth / 2)*(frameWidth / 2) + (frameHeight / 2)*(frameHeight / 2))));
	SpearTipY = y + (sin((SpearAngle  + 315) / 180 * PI)*(sqrt((frameWidth / 2)*(frameWidth / 2) + (frameHeight / 2)*(frameHeight / 2))));

	Mist *mist = new Mist();
	mist->Init(ColorImage, SpearTipX, SpearTipY, FIRE);
	objects.push_back(mist);
}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void PlayerSpear::Render()
{
	GameObject::Render();
	al_draw_tinted_scaled_rotated_bitmap_region(image, image_x, image_y, frameWidth, frameHeight, al_map_rgba_f(193, 148, 62, 1), frameWidth/2, frameHeight/2, x, y, 1, 1, SpearAngle/PI*180, 0);
}