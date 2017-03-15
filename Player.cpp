#include "Player.h"
#include "Dust.h"
#define PI 3.14159265
#define DEGREES(x) int((x)/360.0*0xFFFFFF)
#define RADIANS(x) int((x)/2/M_PI*0xFFFFFF)

Player::Player() {}

void Player::Destroy()
{
	GameObject::Destroy();
}

void Player::Init(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *ref_ColorImage, double ref_x, double ref_y, int ref_dir_x, int ref_dir_y, int ref_vel_x, int ref_vel_y)
{
	frameWidth = 39;
	frameHeight = 96;
	GameObject::Init(ref_x, ref_y, ref_vel_x, ref_vel_y, ref_dir_x, ref_dir_y, frameWidth, frameHeight, PLAYER, TIER1C);
	SetID(PLAYER);
	SetAlive(true);
	SetCollidable(true);
	SetOrigCollidable(true);

	Health = 5;
	Timer = 0;
	ChargeTime = 0;

	maxFrame = 4;
	curFrame = 1;

	MouseAngleRadians = 0;//temporary value

	ColorImage = ref_ColorImage;
	if (image != NULL)
		Player::image = image;
}

void Player::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
	Timer++;
	if (Timer >= 30) {
		Timer = 0;
		curFrame++;
		if (curFrame >= maxFrame)//looping for animation
			curFrame = 0;
	}

	if (dirX != 0 || dirY != 0) {//creation of dust when player moves 
		Dust *dust = new Dust();
		dust->Init(ColorImage, x, y + frameHeight - frameWidth, frameWidth, frameWidth);
		objects.push_back(dust);
	}
}

void Player::Render()
{
	GameObject::Render();
	int fx = curFrame*frameWidth;
	int fy = curAnim*frameHeight;

	al_draw_tinted_bitmap_region(image, al_map_rgba_f(225, 225, 225, 0.5), frameWidth * 4, frameHeight * 0, 36, 18, x, BaseY - 12, 0);//shadow underneath character
	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y, 0);
}

void Player::MoveUp() {
	if (MouseAngleRadians > 3 * PI / 2 ||//animation is based on position of mouse, so that it better aligns with spear direction
		MouseAngleRadians < PI / 2)
		Animation = MOVINGRIGHT;
	else
		Animation = MOVINGLEFT;
	dirY = -1;
	AnimationHandler();
}
void Player::MoveDown() {
	if (MouseAngleRadians > 3 * PI / 2 ||
		MouseAngleRadians < PI / 2)
		Animation = MOVINGRIGHT;
	else
		Animation = MOVINGLEFT;
	dirY = 1;
	AnimationHandler();
}
void Player::MoveLeft() {
	if (MouseAngleRadians > 3 * PI / 2 ||
		MouseAngleRadians < PI / 2)
		Animation = MOVINGRIGHT;
	else
		Animation = MOVINGLEFT;
	dirX = -1;
	AnimationHandler();
}
void Player::MoveRight() {
	if (MouseAngleRadians > 3 * PI / 2 ||
		MouseAngleRadians < PI / 2)
		Animation = MOVINGRIGHT;
	else
		Animation = MOVINGLEFT;
	dirX = 1;
	AnimationHandler();
}

void Player::ResetAnimation(int position)
{
	if (position == 1) {
		if (Animation == MOVINGLEFT || Animation == IDLELEFT || Animation == DASHLEFT || Animation == MOVINGRIGHT || Animation == IDLERIGHT || Animation == DASHRIGHT) {
			if (MouseAngleRadians > 3 * PI / 2 ||
				MouseAngleRadians < PI / 2)
				Animation = IDLERIGHT;
			else
				Animation = IDLELEFT;
		}
		else if (Animation == LUNGELEFT || Animation == LUNGERIGHT) {
			if (MouseAngleRadians > 3 * PI / 2 ||
				MouseAngleRadians < PI / 2)
				Animation = CHARGERIGHT;
			else
				Animation = CHARGELEFT;
		}

		dirY = 0;
		AnimationHandler();
	}
	else {
		dirX = 0;
		//AnimationHandler();
	}


}

void Player::Dash(double MouseAngleRadians) {
	dirX = sin(MouseAngleRadians + PI / 2);
	dirY = cos(MouseAngleRadians + PI / 2);
}
void Player::Charge(int mousex) {
	if (ChargeTime < 60)
		ChargeTime++;

	if (x > mousex)
		Animation = CHARGELEFT;
	else
		Animation = CHARGERIGHT;
	AnimationHandler();
}
void Player::Lunge(double MouseAngleRadians) {
	dirX = sin(MouseAngleRadians + PI / 2);
	dirY = cos(MouseAngleRadians + PI / 2);
	LungeTime--;

	if (dirX < 0)
		Animation = LUNGELEFT;
	else
		Animation = LUNGERIGHT;

	AnimationHandler();
}

//sets up the various variables that come alongside the Animation states. called whenever Animation is changed
void Player::AnimationHandler()
{
	//dimensions and other variables will default to:
	frameWidth = 39;
	frameHeight = 96;
	boundX = 39;
	boundY = 96;
	velX = PLAYERVELX, velY = PLAYERVELY;

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
	else if (Animation == CHARGELEFT) {
		curAnim = 4;
		maxFrame = 1;
	}
	else if (Animation == CHARGERIGHT) {
		curAnim = 5;
		maxFrame = 1;
	}
	else if (Animation == LUNGELEFT) {
		curAnim = (97 * 6) / 69;
		maxFrame = 1;
		frameWidth = 69;
		frameHeight = 72;
		boundX = 69;
		boundY = 72;
		velX = PLAYERVELX*(sqrt(LungeTime));
		velY = PLAYERVELY*(sqrt(LungeTime));
	}
	else if (Animation == LUNGERIGHT) {
		curAnim = (97 * 7) / 69;
		maxFrame = 1;
		frameWidth = 69;
		frameHeight = 72;
		boundX = 69;
		boundY = 72;
		velX = PLAYERVELX*(sqrt(LungeTime));
		velY = PLAYERVELY*(sqrt(LungeTime));
	}
	else if (Animation == DASHLEFT) {
		cout << "DASHLEFT is not finished. get off your ass and finish it, future self." << endl;
	}
	else if (Animation == DASHRIGHT) {
		cout << "DASHRIGHT is not finished. get off your ass and finish it, future self." << endl;
	}

	//sets curFrame based on maxFrame (otherwise, player will temporary disappear when it renders a position that is now greater than maxFrame)
	if (curFrame >= maxFrame) {
		curFrame = 0;
	}
}