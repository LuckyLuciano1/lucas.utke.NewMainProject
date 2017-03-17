#include "PlayerSpear.h"
#include "PlayerSpearFlame.h"
#include  "Player.h"

#define PI 3.14159265
#define DEGREES(x) int((x)/360.0*0xFFFFFF)
#define RADIANS(x) int((x)/2/M_PI*0xFFFFFF)

PlayerSpear::PlayerSpear() {}

void PlayerSpear::Destroy()
{
	GameObject::Destroy();
}

void PlayerSpear::Init(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *ref_ColorImage, double ref_x, double ref_y, double ref_z, double ref_SpearAngleRadians, int ref_SpearState, int ref_ChargeTime, Player *ref_player)
{
	GameObject::Init(ref_x, ref_y, ref_z, PLAYERVELX, PLAYERVELY, 0, 0, 1, 1, PLAYERSPEAR, TIER1C);
	SetCollidable(false);
	SetOrigCollidable(false);

	SetAlive(true);

	SpearAngleRadians = ref_SpearAngleRadians - (sqrt(2) / 2);
	SpearState = ref_SpearState;
	ChargeTime =  ref_ChargeTime;

		frameWidth = 128;// 92;
		frameHeight = 128;// 92;
		image_x = 192;
		image_y = 0;

	SpearTipX = 0;
	SpearTipY = 0;

	player = ref_player;
	ColorImage = ref_ColorImage;
	if (image != NULL)
		PlayerSpear::image = image;
}

void PlayerSpear::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);


	if (SpearState == SPINNING) {
		SpearAngleRadians += PI / (12);// *(ChargeTime / 10));
	}

	x = player->GetX() + player->GetBoundX() / 2;
	y = player->GetY() + player->GetBoundY() / 2;
	
	if (SpearAngleRadians >= 2*PI)//adjusts angle to keep within 2PI range
		SpearAngleRadians -= 2*PI;

	//determines position of spear tip
	//spear positions + x/y width formed by mouse angle and spear (found through cos/sin * hypotenuse)
	SpearTipX = (x + (cos(SpearAngleRadians + (sqrt(2) / 2))*(sqrt((frameWidth / 2)*(frameWidth / 2) + (frameHeight / 2)*(frameHeight / 2)))));
	SpearTipY = (y + (sin(SpearAngleRadians + (sqrt(2) / 2))*(sqrt((frameWidth / 2)*(frameWidth / 2) + (frameHeight / 2)*(frameHeight / 2)))));

	//creates Fire effects at flame tip
	PlayerSpearFlame *playerspearflame = new PlayerSpearFlame();
	playerspearflame->Init(ColorImage, SpearTipX - playerspearflame->GetBoundX() / 2, SpearTipY - playerspearflame->GetBoundY() / 2, 0, ChargeTime);
	objects.push_back(playerspearflame);
}

//does not use animation rows, sprites, etc. unnecessary for basic box sprite.
void PlayerSpear::Render()
{
	GameObject::Render();
	al_draw_tinted_scaled_rotated_bitmap_region(image, image_x, image_y, frameWidth, frameHeight, al_map_rgba_f(1, 1, 1, 1), frameWidth/2, frameHeight/2, x, y, 1, 1, SpearAngleRadians, 0);
}