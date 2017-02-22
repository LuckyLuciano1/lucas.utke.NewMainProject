#include "Player.h"

Player::Player() {}

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
	ActionTimer--;
	cout << ActionTimer << endl;
	if (ActionTimer < 0) {
		ResetAnimation(1);
		StateHandler();
	}
	StateHandler();
}

void Player::Render()
{
	Units::Render();
	int fx = curFrame*frameWidth;
	int fy = curAnim*frameHeight;

	al_draw_tinted_bitmap_region(image, al_map_rgba_f(225, 225, 225, 0.5), frameWidth*4, frameHeight*0, 36, 18, x, BaseY - 12, 0);//shadow underneath character
	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y, 0);
}

void Player::MoveUp() {
	Units::MoveUp();
	Player::StateHandler();
}
void Player::MoveDown() {
	Units::MoveDown();
	Player::StateHandler();
}
void Player::MoveLeft() {
	Units::MoveLeft();
	Player::StateHandler();
}
void Player::MoveRight() {
	Units::MoveRight();
	Player::StateHandler();
}
void Player::ResetAnimation(int position)
{
	Units::ResetAnimation(position);
	Player::StateHandler();
}

void Player::Dash(double MouseAngle) {
	Units::Dash(MouseAngle);
	Player::StateHandler();
}

//sets up the various variables that come alongside the Action states. called whenever Action is changed
void Player::StateHandler()
{
	//dimensions and other variables will default to:
	frameWidth = 39;
	frameHeight = 96;
	boundX = 39;
	boundY = 96;
	//ActionTimer = 1;
	velX = PLAYERVELX;
	velY = PLAYERVELY;
	dirX = 0;
	dirY = 0;
	if (Action == IDLELEFT) {
		dirX = 0;
		dirY = 0;
		curAnim = 0;
		maxFrame = 2;
	}
	else if (Action == IDLERIGHT) {
		dirX = 0;
		dirY = 0;
		curAnim = 1;
		maxFrame = 4;
	}
	else if (Action == MOVINGLEFT) {
		dirX = -1;
		curAnim = 2;
		maxFrame = 2;
	}
	else if (Action == MOVINGRIGHT) {
		dirX = 1;
		curAnim = 3;
		maxFrame = 2;
	}
	else if (Action == MOVINGUPLEFT) {
		dirX = -1;
		dirY = -1;
		curAnim = 2;
		maxFrame = 2;
	}
	else if (Action == MOVINGUPRIGHT) {
		dirX = 1;
		dirY = -1;
		curAnim = 3;
		maxFrame = 2;
	}
	else if (Action == MOVINGDOWNLEFT) {
		dirX = -1;
		dirY = 1;
		curAnim = 2;
		maxFrame = 2;
	}
	else if (Action == MOVINGDOWNRIGHT) {
		dirX = 1;
		dirY = 1;
		curAnim = 3;
		maxFrame = 2;
	}
	else if (Action == DASHLEFT) {
		curAnim = 4;
		maxFrame = 1;
		frameWidth = 69;
		frameHeight = 72;
		//boundX = 69;
		//boundY = 72;
		velX = 20;
		velY = 20;
	}
	else if (Action == DASHRIGHT) {
		curAnim = 5;
		maxFrame = 1;
		frameWidth = 69;
		frameHeight = 72;
		//boundX = 69;
		//boundY = 72;
		velX = 20;
		velY = 20;
	}
}