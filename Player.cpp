#include "Player.h"

Player::Player(){}

void Player::Destroy()
{
	Units::Destroy();
}

void Player::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int copy_dir_x, int copy_dir_y, int copy_vel_x, int copy_vel_y)
{
	frameWidth = 39;
	frameHeight = 96;
	Units::Init(copy_x, copy_y, copy_vel_x, copy_vel_y, copy_dir_x, copy_dir_y, frameWidth, frameHeight, PLAYER, TIER1C, 5, 0, 0);
	SetID(PLAYER);
	SetAlive(true);
	SetCollidable(true);
	SetOrigCollidable(true);

	health = 5;
	timer = 0;

	maxFrame = 14;
	curFrame = 1;//horizontal

	if (image != NULL)
		Player::image = image;
}

void Player::Update(double cameraX, double cameraY)
{
	Units::Update(cameraX, cameraY);
}

void Player::Render()
{
	if (x > 0 &&//will not render if outside console window
		x < SCREENW &&
		y > 0 &&
		y < SCREENH){

		Units::Render();
		int fx = curFrame*frameWidth;
		int fy = curAnim*frameHeight;

		al_draw_tinted_bitmap_region(image, al_map_rgba_f(225, 225, 225, 0.5), 78, 0, 36, 18, x, BaseY - 12, 0);//shadow underneath character
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y, 0);	
		
	}
}

void Player::MoveUp(){ 
	Units::MoveUp();
	Player::StateHandler();
}
void Player::MoveDown(){
	Units::MoveDown();
	Player::StateHandler();
}
void Player::MoveLeft(){
	Units::MoveLeft();
	Player::StateHandler();
}
void Player::MoveRight(){ 
	Units::MoveRight();
	Player::StateHandler();
}
void Player::ResetAnimation(int position)
{
	Units::ResetAnimation(position);
	Player::StateHandler();
}

//sets up the various variables that come alongside the ANIM states
void Player::StateHandler()
{
	if (Action == IDLE) {
		curAnim = 0;
		maxFrame = 2;
	}
	if (Action == LEFT) {
		curAnim = 1;
		maxFrame = 2;
	}
	if (Action == RIGHT) {
		curAnim = 2;
		maxFrame = 2;
	}
}