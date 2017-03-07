#include "Player.h"
#define PI 3.14159265
#define DEGREES(x) int((x)/360.0*0xFFFFFF)
#define RADIANS(x) int((x)/2/M_PI*0xFFFFFF)

Player::Player() {}

void Player::Destroy()
{
	GameObject::Destroy();
}

void Player::Init(ALLEGRO_BITMAP *image, double ref_x, double ref_y, int ref_dir_x, int ref_dir_y, int ref_vel_x, int ref_vel_y)
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
}

void Player::Render()
{
	GameObject::Render();
	int fx = curFrame*frameWidth;
	int fy = curAnim*frameHeight;

	al_draw_tinted_bitmap_region(image, al_map_rgba_f(225, 225, 225, 0.5), frameWidth*4, frameHeight*0, 36, 18, x, BaseY - 12, 0);//shadow underneath character
	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y, 0);
}

void Player::MoveUp() {
	if (Animation == MOVINGRIGHT || Animation == IDLERIGHT || Animation == DASHRIGHT)
		Animation = MOVINGRIGHT;
	else
		Animation = MOVINGLEFT;
	dirY = -1;
	AnimationHandler();
}
void Player::MoveDown() {
	if (Animation == MOVINGRIGHT || Animation == IDLERIGHT || Animation == DASHRIGHT)
		Animation = MOVINGRIGHT;
	else
		Animation = MOVINGLEFT;
	dirY = 1;
	AnimationHandler();
}
void Player::MoveLeft() {
	Animation = MOVINGLEFT;
	dirX = -1;
	AnimationHandler();
}
void Player::MoveRight() {
	Animation = MOVINGRIGHT;
	dirX = 1;
	AnimationHandler();
}

void Player::ResetAnimation(int position)
{
	if (position == 1) {
		if (Animation == MOVINGLEFT)
			Animation = IDLELEFT;
		else if (Animation == MOVINGRIGHT)
			Animation = IDLERIGHT;
		dirY = 0;
		AnimationHandler();
	}
	else{
		dirX = 0;
		//AnimationHandler();
	}

	
}

void Player::Dash(double MouseAngle) {
	dirX = sin((MouseAngle + 90) / 180 * PI);
	dirY = cos((MouseAngle + 90) / 180 * PI);
}
void Player::Charge(int mousex) {
	cout << ChargeTime << endl;
	if (ChargeTime <= 40)
		ChargeTime++;

	if (x > mousex)
		Animation = CHARGELEFT;
	else
		Animation = CHARGERIGHT;
	AnimationHandler();
}
void Player::Lunge(double MouseAngle) {

	velX = PLAYERVELX+ChargeTime;
	velY = PLAYERVELY+ChargeTime;
	dirX = sin((MouseAngle + 90) / 180 * PI);
	dirY = cos((MouseAngle + 90) / 180 * PI);

	if (dirX < 0)
		Animation = LUNGELEFT;
	else
		Animation = LUNGERIGHT;

	AnimationHandler();
	ChargeTime-=2;
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
		velX = PLAYERVELX*(sqrt(ChargeTime))/2;
		velY = PLAYERVELY*(sqrt(ChargeTime))/2;
	}
	else if (Animation == LUNGERIGHT) {
		curAnim = (97 * 7) / 69;
		maxFrame = 1;
		frameWidth = 69;
		frameHeight = 72;
		boundX = 69;
		boundY = 72;
		velX = PLAYERVELX*(sqrt(ChargeTime))/2;
		velY = PLAYERVELY*(sqrt(ChargeTime))/2;
	}
	else if (Animation == DASHLEFT) {
		cout << "DASHLEFT is not finished. get off your ass and finish it, future self." << endl;
	}
	else if (Animation == DASHRIGHT) {
		cout << "DASHRIGHT is not finished. get off your ass and finish it, future self." << endl;
	}

	//sets curFrame based on maxFrame
	if (curFrame > maxFrame)
		curFrame = maxFrame-1;
}