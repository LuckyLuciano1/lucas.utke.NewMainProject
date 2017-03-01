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

	health = 5;
	timer = 0;

	maxFrame = 4;
	curFrame = 1;

	if (image != NULL)
		Player::image = image;
}

void Player::Update(double cameraX, double cameraY, vector<GameObject*> &objects)
{
	GameObject::Update(cameraX, cameraY, objects);
	timer++;
	if (timer >= 30) {
		timer = 0;
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
	if (Action == MOVINGRIGHT || Action == IDLERIGHT || Action == DASHRIGHT)
		Action = MOVINGRIGHT;
	else
		Action = MOVINGLEFT;
	dirY = -1;
	Player::AnimationHandler();
}
void Player::MoveDown() {
	if (Action == MOVINGRIGHT || Action == IDLERIGHT || Action == DASHRIGHT)
		Action = MOVINGRIGHT;
	else
		Action = MOVINGLEFT;
	dirY = 1;
	Player::AnimationHandler();
}
void Player::MoveLeft() {
	Action = MOVINGLEFT;
	dirX = -1;
	Player::AnimationHandler();
}
void Player::MoveRight() {
	Action = MOVINGRIGHT;
	dirX = 1;
	Player::AnimationHandler();
}

void Player::ResetAnimation(int position)
{
	if (position == 1) {
		if (Action == MOVINGLEFT)
			Action = IDLELEFT;
		else if (Action == MOVINGRIGHT)
			Action = IDLERIGHT;
		dirY = 0;
		Player::AnimationHandler();
	}
	else{
		dirX = 0;
		Player::AnimationHandler();
	}

	
}

void Player::Dash(double MouseAngle) {
	dirX = sin((MouseAngle + 90) / 180 * PI);
	dirY = cos((MouseAngle + 90) / 180 * PI);
}

//sets up the various variables that come alongside the Action states. called whenever Action is changed
void Player::AnimationHandler()
{
	//dimensions and other variables will default to:
	//frameWidth = 39;
	//frameHeight = 96;
	//boundX = 39;
	//boundY = 96;

	if (Action == IDLELEFT) {
		curAnim = 0;
		maxFrame = 4;
	}
	else if (Action == IDLERIGHT) {
		curAnim = 1;
		maxFrame = 4;
	}
	else if (Action == MOVINGLEFT) {
		curAnim = 2;
		maxFrame = 4;
	}
	else if (Action == MOVINGRIGHT) {
		curAnim = 3;
		maxFrame = 4;
	}
	else if (Action == DASHLEFT) {
		cout << "DASHLEFT is not finished. get off your ass and finish it, future self." << endl;
	}
	else if (Action == DASHRIGHT) {
		cout << "DASHRIGHT is not finished. get off your ass and finish it, future self." << endl;
	}
}