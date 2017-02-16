#include "Player.h"

Player::Player(){}

void Player::Destroy()
{
	Units::Destroy();
}

void Player::Init(ALLEGRO_BITMAP *image, double copy_x, double copy_y, int copy_dir_x, int copy_dir_y, int copy_vel_x, int copy_vel_y)
{
	frameWidth = 13*3;
	frameHeight = 32*3;
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
	timer++;
	if (timer >= 60){
		timer = 0;
		curFrame++;
		if (curFrame >= maxFrame)
			curFrame= 0;
	}
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

void Player::StateHandler()
{
	if (curAnim == IDLE) {
		frameWidth = 39;
		frameHeight = 96;
		maxFrame = 2;
		curFrame = 0;
	}
	if (curAnim == MOVING) {
		maxFrame = 2;
		curFrame = 0;
		frameWidth = 93;
		frameHeight = 96;
	}
}